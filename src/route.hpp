#include <iostream>
#include "const.hpp"

class Route
{
public:
	std::string path[MAX_DIRECTORIES];

	Route(std::string url);

	static void buildPath(std::string url, std::string (&path)[MAX_DIRECTORIES]);
};

class FileRoute : public Route
{
public:
	int callback;

	FileRoute(std::string url, int callback);
};

class AssetRoute : public Route
{
public:
	std::string directory;

	AssetRoute(std::string url, std::string directory);
};
