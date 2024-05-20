#include <iostream>
#include "const.hpp"
#ifndef HTTP_HPP
#define HTTP_HPP
class HTTP
{
public:
	int contentLength;
	std::string protocol, method, url, languages, contentType, body;
	std::string path[MAX_DIRECTORIES];

	static std::string buildResponse(int status, std::string contentType, std::string content);
	static HTTP* parse(char (&buffer)[BUFFER_SIZE]);
};
#endif
