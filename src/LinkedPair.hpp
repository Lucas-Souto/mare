#ifndef LINKED_PAIR_HPP
#define LINKED_PAIR_HPP
#include <string>

using namespace std;

static const string KEY_NOT_FOUND = "$NIL$";

class LinkedPair
{
public:
	string Key = "", Value = "";
	LinkedPair* Next = nullptr;

	void Set(string key, string value);

	string GetValue(string key);
};
#endif
