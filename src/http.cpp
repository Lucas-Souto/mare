#include "http.hpp"

std::string HTTP::buildResponse(int status, std::string contentType, std::string content)
{
	std::string response = "HTTP/1.1 " + std::to_string(status) + " " + statusText(status) + "\r\nContent-Type: " + contentType;
	response += "\r\nContent-Length: " + std::to_string(content.size()) + "\r\nConnection: Closed\r\n\r\n" + content;

	return response;
}

std::string getText(int& index, char until, char (&buffer)[BUFFER_SIZE])
{
	std::string result = "";

	for (; index < BUFFER_SIZE; index++)
	{
		if (buffer[index] == until)
		{
			index++;

			break;
		}
		
		result += buffer[index];
	}

	return result;
}

void readHeaders(HTTP* request, char (&buffer)[BUFFER_SIZE])
{
	
}

void readBody(HTTP* request, char (&buffer)[BUFFER_SIZE])
{
	
}

HTTP* HTTP::parse(char (&buffer)[BUFFER_SIZE])
{
	int index = 0;
	HTTP* result = new HTTP();
	result->method = getText(index, ' ', buffer);
	result->url = getText(index, ' ', buffer);
	result->protocol = getText(index, '\r', buffer);
	index++;

	readHeaders(result, buffer);
	readBody(result, buffer);

	return result;
}
