#ifndef LINKED_PAIR
#define LINKED_PAIR
#include <string>

using namespace std;

static const string KEY_NOT_FOUND = "$NIL$";

class LinkedPair
{
public:
	string Key, Value;
	LinkedPair* Next;

	LinkedPair(string key, string value);

	string GetValue(std::string key);
};
#endif
