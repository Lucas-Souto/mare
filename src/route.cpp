#include "route.hpp"

Route::Route(std::string url)
{
	printf("Criando rota \"%s\"...\n", url.c_str());
	buildPath(url, this->path);
}

FileRoute::FileRoute(std::string url, int callback) : Route(url)
{
	this->type = 'f';
	this->callback = callback;
}

AssetRoute::AssetRoute(std::string url, std::string directory) : Route(url)
{
	this->type = 'a';
	this->directory = directory;
}

void Route::buildPath(std::string url, std::string (&path)[MAX_DIRECTORIES])
{
	int pathIndex = 0, start = 0, size = 1;
	int length = url.size();
	path[pathIndex] = "/";
	
	if (url.size() > 1)
	{
		for (int i = 1; i < length; i++)
		{
			size++;

			if (url[i] == '/' || i == length - 1 || url[i] == '?')
			{
				if (i == length - 1 && url[i] != '/') size++;

				path[pathIndex] = url.substr(start, size - 1);
				start = i;
				size = 1;
				pathIndex++;
			}

			if (pathIndex >= MAX_DIRECTORIES || url[i] == '?') break;
		}
	}
	else pathIndex++;

	for (; pathIndex < MAX_DIRECTORIES; pathIndex++) path[pathIndex] = "";
}
