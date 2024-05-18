#include <iostream>
#include "const.hpp"

class HTTP
{
public:
	int status, method;
	std::string protocol, url, languages, contentType, body;

	static std::string buildResponse(int status, std::string contentType, std::string content);
	static HTTP* parse(char (&buffer)[BUFFER_SIZE]);
};
