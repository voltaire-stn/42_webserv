/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_headers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 18:41:48 by tsantoni          #+#    #+#             */
/*   Updated: 2021/08/01 11:25:09 by tsantoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <webserv.hpp>

// ********************************************* status line *********************************************

std::string Response::generate_status_line(void)
{
	std::map<int, std::string>::iterator it_msg = _error_msg.find(_status_code);

	// return ("HTTP/" + _request_parser->get__http_version() + PAT_SP + itos(_status_code) + PAT_SP + it_msg->second + PAT_CRLF);
	return ("HTTP/1.1 " + itos(_status_code) + PAT_SP + it_msg->second + PAT_CRLF);
}

// ********************************************* headers utils *********************************************

std::string		Response::formatted_header_response(enum e_resp_fields field)
{
	return (get_response_field_name(field) + PAT_DDOT + PAT_SP + _headers_response[field] + PAT_CRLF);
}

// ********************************************* Server *********************************************

std::string		Response::r_header_server()
{
	_headers_response[R_SERVER] = SERVER_NAME;
	return (formatted_header_response(R_SERVER));
}

// ********************************************* Date *********************************************

std::string		Response::r_header_date()
{
	time_t		rawtime;
	struct tm	*timeinfo;
	char		buffer[MAX_DATE];

	time(&rawtime);
	timeinfo = gmtime(&rawtime);
	strftime(buffer, MAX_DATE, "%a, %d %h %G %X GMT", timeinfo); // Sun, 06 Nov 1994 08:49:37 GMT
	_headers_response[R_DATE] = buffer;
	return (formatted_header_response(R_DATE));
}

// ********************************************* Connection *********************************************

std::string		Response::r_header_connection()
{
	_headers_response[R_CONNECTION] = "close"; // à vérifier
	_headers_response[R_CONNECTION] = "Keep-Alive"; // à vérifier
	return (formatted_header_response(R_CONNECTION));
}

// ********************************************* Content-Length *********************************************

std::string		Response::r_header_content_length()
{
	if (_response_body.length())
		_headers_response[R_CONTENT_LENGTH] = itos(_response_body.length()); // should be binary length when appropriate ?
	else
		_headers_response[R_CONTENT_LENGTH] = "None";
	return (formatted_header_response(R_CONTENT_LENGTH));
}

// ********************************************* Content-Type *********************************************
//
// std::string		Response::exec_cmd(std::string cmd)
// {
// 	std::ostringstream oss;
//
// 	if (!oss) // EXCEPTION A CREER
// 		std::cerr << RED << "Error : can't open osstream" <<  C_RES << std::endl;
// 	int status = std::system(cmd.c_str());
// 	if (WEXITSTATUS(status) != 0)
// 		std::cerr << RED << "Error : can't exec cmd " << cmd <<  C_RES << std::endl; // EXCEPTION
// 	oss << std::ifstream(PATH_CMD_RES).rdbuf();
// 	return oss.str();
// }

void		Response::retrieve_type_mime_charset(std::string str)
{
	unsigned first = str.find(":") + 2;
	unsigned last = str.find(";");

	if (last > str.length() || _translated_path.back() == '/') // si "cannot open file" || directory
	{
		_type_mime = "text/html";
		_charset = "utf-8";
		return ;
	}
	_type_mime = str.substr(first, last - first);
	first = str.find("=") + 1;
	last = str.find("\n");
	_charset = str.substr(first, last - first);
}

std::string		Response::r_header_content_type()
{
	std::string cmd;

	cmd = "file --mime " + _translated_path;
	retrieve_type_mime_charset(exec_cmd(cmd.c_str(), PATH_CMD_RES));
	_headers_response[R_CONTENT_TYPE] = _type_mime + "; charset=" + _charset;
	return (formatted_header_response(R_CONTENT_TYPE));
}