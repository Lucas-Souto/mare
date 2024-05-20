#include <iostream>
#include "const.hpp"
#ifndef ROUTE_HPP
#define ROUTE_HPP
class Route
{
public:
	std::string path[MAX_DIRECTORIES];

	Route(std::string url);

	char getType() { return type; }
	static void buildPath(std::string url, std::string (&path)[MAX_DIRECTORIES]);
protected:
	char type;
};

class FileRoute : public Route
{
public:
	int callback = -1;

	FileRoute(std::string url, int callback);
};

class AssetRoute : public Route
{
public:
	std::string directory;

	AssetRoute(std::string url, std::string directory);
};
#endif
