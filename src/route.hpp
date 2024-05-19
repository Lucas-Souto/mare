#include <iostream>
#include "const.hpp"

class Route
{
public:
	std::string path[MAX_DIRECTORIES];

	Route(std::string path);
};

class FileRoute : public Route
{
public:
	int callback;

	FileRoute(std::string path, int callback);
};

class AssetRoute : public Route
{
public:
	std::string directory;

	AssetRoute(std::string path, std::string directory);
};
