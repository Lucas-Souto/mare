#include "Request.hpp"
#include "utils.hpp"

string getText(int &index, const char until, char (&buffer)[BUFFER_SIZE])
{
	string result = "";

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

void readHeaders(Request* request, int &index, char (&buffer)[BUFFER_SIZE])
{
	string key, value;

	while (index < BUFFER_SIZE)
	{
		if (buffer[index] == '\r' && (index + 1 < BUFFER_SIZE && buffer[index + 1] == '\n')) return;

		key = getText(index, ':', buffer);
		value = getText(index, '\r', buffer);
		index++;

		if (key == "Host") request->Host = value;
		else if (key == "Content-Length") request->ContentLength = stoi(value);
		else if (key == "Content-Type") request->ContentType = value;
		else if (key == "Accept-Language") request->Languages = value;
	}
}

void readBody(Request* request, int &index, char (&buffer)[BUFFER_SIZE])
{
	if (request->ContentLength <= 0) return;

	request->Body = "";
	int size = min(request->ContentLength, BUFFER_SIZE - index);

	request->Body.assign(buffer + index, size);
}

Request* Request::ParseRequest(char (&buffer)[BUFFER_SIZE])
{
	int index = 0;
	Request* request = new Request();
	request->Method = getText(index, ' ', buffer);
	request->URL = getText(index, ' ', buffer);
	request->Target = new Route(request->URL, -1);
	request->Protocol = getText(index, '\r', buffer);
	index++;

	readHeaders(request, index, buffer);
	readBody(request, index, buffer);

	return request;
}
