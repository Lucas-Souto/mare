#include "LinkedPair.hpp"

LinkedPair::LinkedPair(string key, string value)
{
	Key = key;
	Value = value;
	Next = nullptr;
}

string LinkedPair::GetValue(string key)
{
	if (Key == key) return Value;
	else if (Next != nullptr) return Next->GetValue(key);

	return KEY_NOT_FOUND;
}
