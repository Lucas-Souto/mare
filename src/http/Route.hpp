#ifndef ROUTE_HPP
#define ROUTE_HPP
#include <vector>
#include <string>

using namespace std;

class Route
{
public:
	const vector<string> Path;

	const int Callback;
	const string Directory;

	Route(string url, int callback);
	Route(string url, string directory);

	static Route* FindMatchingRoute(Route* compare, vector<Route*>* from);
};

#endif
