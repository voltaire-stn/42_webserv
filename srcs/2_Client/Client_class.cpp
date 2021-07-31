/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 18:41:33 by tsantoni          #+#    #+#             */
/*   Updated: 2021/07/31 11:45:09 by tsantoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <webserv.hpp>

// ********************************************* ::recv => request en std::string *********************************************

void Client::receive_request(void)
{
	char	buffer[MAX_RCV];

	try
	{
		_bytes_read = ::recv(_socket, buffer, MAX_RCV - 1, 0);
		if (_bytes_read == -1)
			throw Exceptions::RecvFailure();
		std::cout << GREEN << "Request of size " << _bytes_read << " received :" <<  C_RES << std::endl;
		buffer[_bytes_read] = '\0';
		_request.assign(buffer, _bytes_read);
	}
	catch (Exceptions::RecvFailure & e)
	{
		std::cerr << RED << e.what() <<  C_RES << std::endl;
	}

	return ;
}

// ********************************************* parse + check request *********************************************

void	Client::check_method(void)
{
	for (v_string_iterator it = this->_request_parser->_methods_implemented.begin();
		it != this->_request_parser->_methods_implemented.end(); it++)
	{
		if (this->_request_parser->get__method() == *it)
			return ;
	}
	throw (HTTP_ErrorStatusException(501)); // Not Implemented
}
void	Client::check_http_version(void)
{
	if (this->_request_parser->get__http_version() != "1.1")
		throw (HTTP_ErrorStatusException(505)); // HTTP Version Not Supported
}

void	Client::check_request(void)
{
	_request_parser = new RequestParser(_request, _bytes_read); // delete in generate_response :)
	_request_parser->print_request_info();
	_status_code = _request_parser->get__status();
	if (_status_code == 200)
	{
		try { check_method(); }
		catch (HTTP_ErrorStatusException & e) {
			_status_code = e.get__status();
		}
		try { check_http_version(); }
		catch (HTTP_ErrorStatusException & e) {
			_status_code = e.get__status();
		}
	}
	return ;
}

// ********************************************* parse path *********************************************

void	Client::parse_parameters(void)
{
	std::string p = _query_string;
	int sep = 0;
	int eq = 0;

	while (sep > -1)
	{
		sep = p.find("&");
		eq = p.find("=");
		_parameters[p.substr(0, eq)] = p.substr(eq + 1, sep - (eq + 1));
		p.erase(0, sep + 1);
	}
}

void		Client::apply_location(void)
{
	// si path contient location, lier _applied location à la location
	std::string							rsc = _request_parser->get__resource();
	std::map<std::string, Location *>	m = _config.getLocations();
	Location 							*l = NULL;;

	// TO PRECISE iterate through maps && search from end to beginning
	std::size_t found = rsc.find("/documents");
	if (rsc == "/")
		l = m["/"];
	else if (found == 0)
		l = m["/documents"];
	_applied_location = l;
	// if (_applied_location)
	// {
	// 	std::cerr << C_G_RED << "[ DEBUG Uri       ] " << C_RES << _applied_location->getUri() << std::endl;
	// 	std::cerr << C_G_RED << "[ DEBUG RootLoc   ] " << C_RES << _applied_location->getRootLoc() << std::endl;
	// 	std::cerr << C_G_RED << "[ DEBUG Autoindex ] " << C_RES << _applied_location->getAutoindex() << std::endl;
	// }
}

void		Client::construct_full_path(void)
{
	std::string rsc = _request_parser->get__resource();

	/*
		si path contient alias, remplacer par location uri - grace a map d'alias-uri_location
	*/
	apply_location();
	/*
		TO DO : remplacer les caractères spéciaux
			- ex : %20 " "
			- ex : %C3%A7 "ç"
	*/
	// Theo remaster - // remplir query_string
	if (rsc.find("?") < rsc.length())
	{
		_query_string = rsc.substr(rsc.find("?") + 1);
		rsc.erase(rsc.find("?"));
	}
	rsc = "./html" + rsc;
	// if directory
	if (rsc.back() == '/')
	{
		// appliquer un index, si on le trouve pas ET QUE autoindex on, generer autoindex
		rsc += "index.html";
	}
	_full_path = rsc;
	if (!_query_string.empty())
		parse_parameters();
	return;
}

// ********************************************* read resource *********************************************

void Client::read_resource(void)
{
	std::ifstream ifs(_full_path);
	char c;

	_page_content = "";
	if (!ifs)
		_status_code = 404;
	else
	{
		while (ifs >> std::noskipws >> c)
			_page_content += c;
	}
	std::cerr << BROWN << "status code: " << _status_code << C_RES << std::endl;
	if (ifs)
		ifs.close();
	return ;
}

// ********************************************* generate response *********************************************

void Client::generate_response(void)
{
	_response = new Response(_config, _status_code, _page_content, _full_path, *_request_parser);
	_response->generate();
	if (_request_parser != NULL)
		delete _request_parser;
}

// ********************************************* ::send response *********************************************

void Client::send_response(void)
{
	int	bytes_sent = 0;
	int len = (_response->getResponse().length() + 1);

	char buffer[len];
	memcpy(buffer, _response->getResponse().c_str(), len);
	try
	{
		bytes_sent = ::send(_socket, buffer, len, 0);
		if (bytes_sent == -1)
			throw Exceptions::SendFailure();
		std::cout << GREEN << "Response of size " << bytes_sent << " sent :" <<  C_RES << std::endl;
	}
	catch (Exceptions::SendFailure & e)
	{
		std::cerr << RED << e.what() <<  C_RES << std::endl;
	}
}

// ********************************************* main - treat client *********************************************

void Client::treat_client(void)
{
	receive_request();
	check_request();
	construct_full_path();
	read_resource();
	generate_response();
	send_response();

	std::cout <<  _response->getResponseHeader() << std::endl;
	// std::cout <<  _response->getResponseBody() << std::endl;

	return ;
}
