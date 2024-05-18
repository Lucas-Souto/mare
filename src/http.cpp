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

void readHeaders(HTTP* request, int& index, char (&buffer)[BUFFER_SIZE])
{
	std::string key, value;

	while (index < BUFFER_SIZE)
	{
		if (buffer[index] == '\r' && (index + 1 < BUFFER_SIZE && buffer[index] == '\n')) return;

		key = getText(index, ':', buffer);
		value = getText(index, '\r', buffer);
		index++;

		if (key == "Content-Length") request->contentLength = std::stoi(value);
		else if (key == "Content-Type") request->contentType = value;
		else if (key == "Accept-Language") request->languages = value;
	}
}

void readBody(HTTP* request, int& index, char (&buffer)[BUFFER_SIZE])
{
	if (request->contentLength <= 0) return;

	request->body = "";

	for (; index < index + request->contentLength && index < BUFFER_SIZE; index++) request->body += buffer[index];	
}

HTTP* HTTP::parse(char (&buffer)[BUFFER_SIZE])
{
	int index = 0;
	HTTP* result = new HTTP();
	result->method = getText(index, ' ', buffer);
	result->url = getText(index, ' ', buffer);
	result->protocol = getText(index, '\r', buffer);
	index++;

	readHeaders(result, index, buffer);
	readBody(result, index, buffer);

	return result;
}
