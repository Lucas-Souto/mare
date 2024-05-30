#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include "Route.hpp"

using namespace std;

static const int BUFFER_SIZE = 30720;

class Request
{
public:
	int ContentLength;
	string Protocol, Method, URL, Host, ContentType, Languages, Body;
	Route* Target;

	static Request* ParseRequest(char (&buffer)[BUFFER_SIZE]);
};
#endif
