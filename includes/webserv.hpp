
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
# include <sys/stat.h>
# include <dirent.h>
# include <sys/time.h>
// #include <pstream.h>
//
typedef int SOCKET;
# define MAX_RCV	4096
# define MAX_DATE	80
# define SUCCESS	0
# define FAILURE	-1

// # define START_FUN deep++; dprintf(1, "%*s[ %s%s%s ] "C_G_CYAN"start"C_RES"", deep * 4, "", C_G_GRAY, __FUNCTION__, C_RES); print_center_line((int)strlen((char *)__FUNCTION__), (int)strlen("start"))
// # define DEBUG_MSG(X) dprintf(1, "%*s[ %s%s%s ] "C_G_WHITE"%s on \'%c\'"C_RES"\n", deep * 4, "", C_G_GRAY, __FUNCTION__, C_RES, X, _config_raw[_head])
// # define END_FUN(X) dprintf(1, "%*s[ %s%s%s ] %s", deep * 4, "", C_G_GRAY, __FUNCTION__, C_RES, X == FAILURE ? C_G_RED"FAILURE"C_RES : X == SUCCESS ? C_G_GREEN"SUCCESS"C_RES : "code "#X); print_center_line((int)strlen((char *)__FUNCTION__), (int)strlen(X == FAILURE ? "FAILURE" : X == SUCCESS ? "SUCCESS" : "code "#X)); deep--
// # define DIGEST_DEBUG(X) dprintf(1, "%*s[ %s%s%s ] \'"C_G_WHITE"%s"C_RES"\'\n", (deep + 1) * 4, "", C_G_YELLOW, __FUNCTION__, C_RES, X)

# define START_FUN // deep++; dprintf(1, "%*s[ %s%s%s ] "C_G_CYAN"start"C_RES"", deep * 4, "", C_G_GRAY, __FUNCTION__, C_RES); print_center_line((int)strlen((char *)__FUNCTION__), (int)strlen("start"))
# define DEBUG_MSG(X) // dprintf(1, "%*s[ %s%s%s ] "C_G_WHITE"%s on \'%c\'"C_RES"\n", deep * 4, "", C_G_GRAY, __FUNCTION__, C_RES, X, _config_raw[_head])
# define END_FUN(X) // dprintf(1, "%*s[ %s%s%s ] %s", deep * 4, "", C_G_GRAY, __FUNCTION__, C_RES, X == FAILURE ? C_G_RED"FAILURE"C_RES : X == SUCCESS ? C_G_GREEN"SUCCESS"C_RES : "code "#X); print_center_line((int)strlen((char *)__FUNCTION__), (int)strlen(X == FAILURE ? "FAILURE" : X == SUCCESS ? "SUCCESS" : "code "#X)); deep--
# define DIGEST_DEBUG(X) // dprintf(1, "%*s[ %s%s%s ] \'"C_G_WHITE"%s"C_RES"\'\n", (deep + 1) * 4, "", C_G_YELLOW, __FUNCTION__, C_RES, X)

# define SVN(X) "head_save_"#X
# define VN(X) head_save_##X
# define SAVE_HEAD(X) size_t VN(X) = this->_head
# define RESAVE_HEAD(X) VN(X) = this->_head
# define LOAD_HEAD(X) this->_head = VN(X)

# define SET_ERROR _error.msg = std::string(__FUNCTION__) + std::string(" > ") + _error.msg; _error.head = _head;
# define RESET_ERROR _error.msg = "";

# define MIN(X, Y) X < Y ? X : Y
# define MAX(X, Y) X > Y ? X : Y
# define ABS(X) X < 0 ? -X : X
# define PATTERNLINE "---------------------------------------------------------------------------------------------"


# include <Exceptions.hpp>
# include <0_Config.hpp>
# include <0_Location.hpp>
# include <ConfigParser.hpp>
# include <1_Server.hpp>
# include <2_Client.hpp>
# include <3_RequestParser.hpp>
# include <4_Response.hpp>
# include <5_Cgi.hpp>
# include <utils.hpp>

typedef std::map<std::string, std::string>::iterator	headers_iterator;
typedef std::vector<std::string>::iterator				v_string_iterator;

#endif
