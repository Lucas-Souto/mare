#ifndef ROUTE
#define ROUTE
#include <list>
#include <string>

using namespace std;

class Route
{
public:
	const list<string>* Path;

	const int Callback;
	const string Directory;

	Route(string url, int callback);
	Route(string url, string directory);
};

#endif
