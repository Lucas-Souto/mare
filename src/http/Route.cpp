#include "Route.hpp"
#include <iostream>

vector<string> generatePath(string url)
{
	vector<string>* result = new vector<string>();
	int start = 0, size = 0, length = url.size();

	if (length <= 1) result->push_back("/");
	else
	{
		for (int i = 1; i < length; i++)
		{
			bool push = false;
			size++;

			if (url[i] == '/' || url[i] == '?') push = true;
			else if (i == length - 1)
			{
				push = true;
				size++;
			}

			if (push)
			{
				result->push_back(url.substr(start, size));

				size = 0;
				start = i;
			}

			if (url[i] == '?') break;
		}
	}
	
	return *result;
}

Route::Route(string url, int callback) : Path(generatePath(url)), Callback(callback), Directory("") { }
Route::Route(string url, string directory) : Path(generatePath(url)), Callback(-1), Directory(directory) { }

Route* Route::FindMatchingRoute(Route* compare, vector<Route*>* from)
{
	int match = 0, lastMatch = 0;// Quantidade de subrotas que deram match
	Route* result = nullptr;
	int compareLength = compare->Path.size(), routeLength;

	for (Route* route : *from)
	{
		routeLength = route->Path.size();

		if (compare->Path[0] == "/" && route->Path[0] == "/")
		{
			result = route;

			break;
		}
		else if (routeLength > compareLength) continue;
		else for (int i = 0; i < compareLength; i++)
		{
			if (i >= routeLength)
			{
				match = 0;

				continue;
			}
			else
			{
				if (compare->Path[i] == route->Path[i])
				{
					match++;

					if (match > lastMatch)
					{
						lastMatch = match;
						result = route;
					}
				}
				else
				{
					match = 0;

					continue;
				}
			}
		}
	}

	return result;
}
