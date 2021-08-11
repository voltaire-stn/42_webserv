/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.class.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tharchen <tharchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 16:35:48 by tharchen          #+#    #+#             */
/*   Updated: 2021/08/11 11:40:34 by tharchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

char	**g_trace = NULL;
int		g_btsize = 0;

void	RequestParser::bc_s(void)
{
	void	*array[1024];

	g_btsize = backtrace(array, 1024);
	if (g_trace)
		delete (g_trace);
	g_trace = backtrace_symbols(array, g_btsize);
}

void	RequestParser::print_center_line(int fn, int msg)
{
	size_t i = MAX((int)((int)this->_head - (int)30), 0);
	dprintf(1, " %-.*s %3lu {", 80 - (deep * 4) - fn - msg, PATTERNLINE, i);
	for (; this->_request_raw[i] && i < this->_head + 30; i++)
	{
		if (i >= this->_head_last_digest && i <= this->_head)
			dprintf(1, C_B_BLUE);
		if (i == this->_head)
			dprintf(1, C_B_RED);
		else
			dprintf(1, C_G_GRAY);
		print_char_printf(this->_request_raw[i]);
		dprintf(1, C_RES);
	}
	dprintf(1, "} %2lu\n", i);
}

void	RequestParser::debug_print_line(void)
{
	std::cout << "head: " << this->_head << " \'";
	print_char(this->_request_raw[this->_head]);
	std::cout << "\'" << std::endl;
	for (size_t i = 0; this->_request_raw[i]; i++)
	{
		if (i >= this->_head_last_digest && i <= this->_head)
			std::cout << C_B_BLUE;
		if (i == this->_head)
			std::cout << C_B_RED;
		else
			std::cout << C_G_GRAY;
		print_char(this->_request_raw[i]);
		std::cout << C_RES;
	}
	std::cout << std::endl;
}

void	RequestParser::bc_p(void)
{
	std::cout << "backtrace: " << std::endl;
	if (g_trace != NULL)
	{
		for (int i = 1; i < g_btsize - 8; i++)
		{
			// std::cout << g_trace[i] << std::endl;
			int j = 64 + 13;
			for (;isdigit(g_trace[i][j]); j++) { }
			int k = j;
			for (;g_trace[i][j] && g_trace[i][j - 1] != 'E' && g_trace[i][j] != 'v'; j++) { }
			g_trace[i][j - 1] = '\0';
			std::cout << "    " << g_btsize - i - 8 << ": " << &g_trace[i][k] << std::endl;
		}
	}
}

int				RequestParser::eat(int to_eat)
{
	if (this->_head >= static_cast<size_t>(this->_bytes_read))
		throw (Exceptions::LexerException("eat ending"));
	else if (this->_request_raw[this->_head] == to_eat)
	{
		this->_head++;
		return (SUCCESS);
	}
	return (FAILURE);
}

void			RequestParser::init_digest(void)
{
	this->_head_last_digest = this->_head;
}

void			RequestParser::digest(std::string & target)
{
	try
	{
		target = this->_request_raw.substr(
			this->_head_last_digest,				// pos
			this->_head - this->_head_last_digest	// length
		);
		DIGEST_DEBUG(target.c_str());
	}
	catch (std::exception & e) {
		std::cout << "error: substr: " << e.what() << std::endl;
	}
}

void			RequestParser::start_parsing(void)
{
	// parser entry point
	try { HTTP_message(); }
	catch (std::exception & e)
	{
		this->_status_code = 400;
		std::cout << C_G_RED << "error: " << C_G_WHITE << _error.msg << C_RES << std::endl;
		this->_head = _error.head;
		this->print_center_line((int)strlen((char *)__FUNCTION__), (int)strlen(""));
	}
}

void			RequestParser::print_request_info(void)
{
	std::cout << "request info:" << std::endl;
	std::cout << "    " << C_G_CYAN << "method " << C_RES << "          : " << this->get__method() << std::endl;
	std::cout << "    " << C_G_CYAN << "path " << C_RES << "            : " << "[" << this->get__resource() << "]" << std::endl;
	std::cout << "    " << C_G_CYAN << "http_version " << C_RES << "    : " << this->get__http_version() << std::endl;
	if (this->_body_size > 0)
	{
		std::cout << "    " << C_G_CYAN << "body size " << C_RES << "       : " << this->_body_size << std::endl;
		dprintf(1, "    "C_G_CYAN"body "C_RES"            : [");
		write(1, this->get__body().c_str(), this->_body_size);
		dprintf(1, "]\n");
	}
	else
		std::cout << "    " << C_G_CYAN << "body " << C_RES << "            : " << "no body" << std::endl;
	std::cout << "    " << C_G_CYAN << "headers  " << C_RES << "        : " << std::endl;
	for (headers_iterator it = this->_header_fields.begin();
		it != this->_header_fields.end(); it++)
	{
		std::cout << "        [" << C_G_BLUE << it->first << C_RES << "] -> [" << it->second << "]" << std::endl;
	}
	std::cout << "-------------------------------------------------------" << std::endl;
}
