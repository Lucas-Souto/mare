#ifndef MESSAGE
#define MESSAGE
#include <list>
#include <string>
#include "Route.hpp"

using namespace std;

static const int BUFFER_SIZE = 30720;

class Message
{
public:
	int ContentLength;
	string Protocol, Method, URL, Languages, ContentType, Body;
	Route* Target;

	static Message* ParseRequest(char (&buffer)[BUFFER_SIZE]);
	static string BuildResponse(int status, string contentType, string content);
};
#endif
