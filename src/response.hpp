#include <iostream>
#include <fstream>
#include <filesystem>

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

}

std::string getContentType(const char* filePath);
std::string getBody(const char* filePath);

std::string render(const char* filePath, const char* tKeys[], const char* tValues[], int tSize);
#endif
