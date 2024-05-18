#include "http.hpp"

std::string HTTP::buildResponse(int status, std::string contentType, std::string content)
{
	std::string response = "HTTP/1.1 " + std::to_string(status) + " " + statusText(status) + "\r\nContent-Type: " + contentType;
	response += "\r\nContent-Length: " + std::to_string(content.size()) + "\r\nConnection: Closed\r\n\r\n" + content;

	return response;
}

HTTP* HTTP::parse(char (&buffer)[BUFFER_SIZE])
{
	HTTP* result = new HTTP();

	return result;
}
