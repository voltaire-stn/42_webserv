/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.class.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tharchen <tharchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 15:33:57 by tharchen          #+#    #+#             */
/*   Updated: 2021/07/24 18:11:15 by tharchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_CLASS_HPP
# define REQUESTPARSER_CLASS_HPP
# include <execinfo.h>
# include <webserv.hpp>

# define PAT_SP		" "
# define PAT_HTAB	"\t"
# define PAT_CR		"\r"
# define PAT_LF		"\n"
# define PAT_DOT	"."
# define PAT_DDOT	":"
# define PAT_HYPHEN	"-"
# define PAT_SLASH	"/"
# define PAT_CRLF	PAT_CR""PAT_LF

// DEBUG BACKTRACE

class LexerException: public std::exception
{
	private:
		std::string _msg;
	public:
		LexerException(std::string msg): _msg(msg) {}  // constructor
		virtual ~LexerException(void) throw() {} // destructor
		virtual const char *what() const throw() { return (this->_msg.c_str()); }
};

class RequestParser
{
	typedef std::map<std::string, std::string>::iterator headers_iterator;
	protected:
		// PROTECTED MEMBERS ATRIBUTE : PROMA

		// PROTECTED MEMBERS FUNCTION : PROMF

	private:
		// PRIVATE MEMBERS ATRIBUTE : PRIMA
		std::string		_request_raw;
		size_t			_head;
		size_t			_head_last_digest;

		std::string		_method;
		std::string		_resource;
		std::string		_http_version;
		std::string		_body;
		std::map<std::string, std::string>	_header_fields;

		// PRIVATE MEMBERS FUNCTION : PRIMF
		RequestParser(void);  // constructor

		// debug
		void	bc_s(void);
		void	bc_p(void);

		// entry point
		void	start_parsing(void);

		// parsing tools
		int		eat(int to_eat);
		void	init_digest(void);
		void	digest(std::string & target);

		// lexing functions
		void	CHAR(char c);
		void	CR(void);
		void	LF(void);
		void	SP(void);
		void	HTAB(void);
		void	RANGE(char start, char end);
		void	ALPHA(void);
		void	DIGIT(void);
		void	HEXDIG(void);
		void	OCTET(void);
		void	VCHAR(void);
		void	sub_delims(void);
		void	tchar(void);
		void	obs_text(void);

		// parsing functions
		void	CRLF(void);
		void	OWS(void);
		void	dec_octet(void);
		void	IPvFuture(void);
		void	h16(void);
		void	IPv4address(void);
		void	ls32(void);
		void	IPv6address(void);
		void	segment_nz(void);
		void	reg_name(void);
		void	IP_literal(void);
		void	path_rootless(void);
		void	path_absolute(void);
		void	path_abempty(void);
		void	pct_encoded(void);
		void	unreserved(void);
		void	port(void);
		void	host(void);
		void	userinfo(void);
		void	hier_part(void);
		void	scheme(void);
		void	pchar(void);
		void	segment(void);
		void	authority(void);
		void	absolute_URI(void);
		void	query(void);
		void	absolute_path(void);
		void	HTTP_name(void);
		void	asterisk_form(void);
		void	authority_form(void);
		void	absolute_form(void);
		void	origin_form(void);
		void	obs_fold(void);
		void	field_vchar(void);
		void	field_content(void);
		void	token(void);
		void	HTTP_version(void);
		void	request_target(void);
		void	method(void);
		void	field_value(void);
		void	field_name(void);
		void	request_line(void);
		void	message_body(void);
		void	header_field(void);
		void	start_line(void);
		void	HTTP_message(void);
	public:
		// CONSTRUCTOR / DESTRUCTOR : CD
		RequestParser(std::string request_raw); // param constructor
		RequestParser(const RequestParser &src);  // constructor by copy
		virtual ~RequestParser(void); // destructor
		// OVERLOADED OPERATORS : OVOP
		RequestParser	&operator=(const RequestParser &rhs);
		// GETTERS / SETTERS : GS
		std::string		get__method(void) const;
		std::string		get__resource(void) const;
		std::string		get__http_version(void) const;
		std::string		get__body(void) const;
		// PUBLIC MEMBERS ATRIBUTE : PUMA
		void			print_request_info(void);

		// PUBLIC MEMBERS FUNCTION : PUMF

};

std::ostream	&operator<<(std::ostream &o, const RequestParser &i);
#endif

/*
** ***********************************************
** ******************* METHODS *******************
**
** ***************** implemented *****************
** GET
** POST
** DELETE
** *************** none implemented **************
** COPY
** OPTIONS
** HEAD
** LINK
** MOVE
** PUT
** PATCH
** TRACE
** UNLINK
** WRAPPED
**
**
** ******************* HEADERS *******************
**

** **************** General-Header ***************
** Cache-Control
** Connection
** Date
** Forwarded
** Keep-Alive
** MIME-Version
** Pragma
** Upgrade
**
** **************** Request-Header ***************
** Accept
** Accept-Charset
** Accept-Encoding
** Accept-Language
** Authorization
** From
** Host
** If-Modified-Since
** Proxy-Authorization
** Range
** Referer
** Unless
** User-Agent
**
**
** **************** Response-Header ***************
* Accept-Ranges
* Age
* ETag
** Location
** Proxy-Authenticate
** Public
** Retry-After
** Server
* Vary
** WWW-Authenticate
**
** ***************** Entity-Header ***************
** Allow
** Content-Encoding
** Content-Language
** Content-Length
** Content-MD5
** Content-Range
** Content-Type
** Content-Version
** Derived-From
** Expires
** Last-Modified
** Link
** Title
** Transfer-Encoding
** URI-header
** ***********************************************
*/


/*
___________________________________ GENERAL ___________________________________
Cache-Control            ; Section 14.9
Connection               ; Section 14.10
Date                     ; Section 14.18
Pragma                   ; Section 14.32
Trailer                  ; Section 14.40
Transfer-Encoding        ; Section 14.41
Upgrade                  ; Section 14.42
Via                      ; Section 14.45
Warning

*/

