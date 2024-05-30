#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include "../LinkedPair.hpp"

using namespace std;

string statusText(int status);// Thanks to https://github.com/j-ulrich/http-status-codes-cpp

string getContentType(const char* filePath);
string getBody(const char* filePath);
string render(const char* filePath, LinkedPair* dict);

string buildResponse(int status, string contentType, string content);
#endif
