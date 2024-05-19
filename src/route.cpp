#include "route.hpp"

Route::Route(std::string path)
{
	printf("Criando rota \"%s\"...\n", path.c_str());

	int pathIndex = 0, start = 0, size = 1;
	int length = path.size();
	this->path[pathIndex] = "/";

	if (path.size() > 1)
	{
		for (int i = 1; i < length; i++)
		{
			size++;

			if (path[i] == '/' || i == length - 1)
			{
				if (i == length - 1 && path[i] != '/') size++;

				this->path[pathIndex] = path.substr(start, size - 1);
				start = i;
				size = 1;
				pathIndex++;
			}
		}
	}
}

FileRoute::FileRoute(std::string path, int callback) : Route(path)
{
	this->callback = callback;
}

AssetRoute::AssetRoute(std::string path, std::string directory) : Route(path)
{
	this->directory = directory;
}
