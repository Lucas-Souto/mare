#ifndef HTML_HPP
#define HTML_HPP
#include <string>
#include <vector>
#include <list>

using namespace std;

static const char VAR_INDICATOR = '$', STATEMENT_INDICATOR = '#', SLASH = '\\', C_NULL = '\0';
static const string CONTENT_KEY = "$content$";

class HTML
{
public:
    const char* ID;
	string Style = "", Script = "";
    list<string> Pieces, Import;

    HTML(const char* id, const string content);
};

HTML* getHTML(const char* id, const char* filePath, const vector<HTML*> list, bool createIfNotExists);
#endif
