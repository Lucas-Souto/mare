#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>

#ifndef RESPONSE_HPP
#define RESPONSE_HPP
struct Response
{
public:
	int status;
	std::string contentType, body;
};

class HTML
{
public:
	const char* id;
	std::list<std::string> pieces;

	HTML(const char* id, std::string content);
};

class CharDict
{
public:
	std::string key, value;
	CharDict* next;

	CharDict(int size);

	void set(std::string key, std::string value);
};

const char* findValue(const char* key, CharDict* first);

std::string getContentType(const char* filePath);
std::string getBody(const char* filePath);

std::string render(const char* filePath, CharDict* dict);
#endif
