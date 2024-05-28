#include "Route.hpp"

list<string>* generatePath(string url)
{
	list<string>* result = new list<string>();
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
	
	return result;
}

Route::Route(string url, int callback) : Path(generatePath(url)), Callback(callback), Directory("") { }
Route::Route(string url, string directory) : Path(generatePath(url)), Callback(-1), Directory(directory) { }
