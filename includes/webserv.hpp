/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 18:42:52 by tsantoni          #+#    #+#             */
/*   Updated: 2021/07/26 15:25:11 by tharchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <netinet/ip.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <cstdlib>
# include <ctime>
# include <map>
# include "color.hpp"
# include <vector>
// #include <pstream.h>
//
typedef int SOCKET;
# define MAX_RCV	4096
# define MAX_DATE	80
# define SUCCESS	0
# define FAILURE	-1

# include "utils.hpp"
# include "Config.hpp"
# include "RequestParser.class.hpp"
# include "Cgi.hpp"
# include "Response.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Exceptions.hpp"

typedef std::map<std::string, std::string>::iterator	headers_iterator;
typedef std::vector<std::string>::iterator				v_string_iterator;

#endif
