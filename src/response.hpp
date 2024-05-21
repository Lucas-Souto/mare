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

std::string getContentType(const char* filePath);
std::string toBase64(const char* filePath);

std::string render(const char* filePath);
#endif
