#include "HTML.hpp"
#include "http/utils.hpp"
#include "LinkedPair.hpp"
#include "Server.hpp"
#include <string.h>

string getTag(int start, string content)
{
	string result = "";

	for (int i = start; i < content.size(); i++)
	{
		if (content[i] == ' ' || content[i] == '>' || content[i] == '/') break;
		else result += content[i];
	}

	return result;
}

int getTagArgs(string tag, int currentIndex, string content, LinkedPair* dict)
{
	LinkedPair* currentPair = dict;
	bool readingContent, readingValue;

	for (; currentIndex < content.size(); currentIndex++)
	{
		if (readingContent)
		{
			if (content[currentIndex] == '<' && content.size() - 1 > currentIndex + tag.size() + 1
				&& content.substr(currentIndex, tag.size() + 3) == "</" + tag + ">")
			{
				currentIndex += tag.size() + 3;

				break;
			}

			currentPair->Value += content[currentIndex];
		}
		else
		{
			if (content[currentIndex] == '/' && content.size() - 1 > currentIndex && content[currentIndex + 1] == '>')
			{
				currentIndex++;

				break;
			}
			else if (content[currentIndex] == '>')
			{
				readingContent = true;
				currentPair->Key = CONTENT_KEY;

				continue;
			}

			if (!readingValue)
			{
				if (content[currentIndex] != ' ')
				{
					if (currentPair->Key != "" && content[currentIndex] == '=')
					{
						readingValue = true;
						currentPair->Key = VAR_INDICATOR + currentPair->Key + VAR_INDICATOR;
					}
					else currentPair->Key += content[currentIndex];
				}
			}
			else
			{
				if (currentPair->Value == "")
				{
					if (content.size() - 1 > currentIndex && content[currentIndex] == '"')
					{
						currentPair->Value += content[currentIndex + 1];
						currentIndex++;
					}
				}
				else
				{
					if (content[currentIndex] == SLASH && content.size() - 1 > currentIndex && content[currentIndex + 1] == '"')
					{
						currentIndex++;
						currentPair->Value += content[currentIndex];
					}
					else if (content[currentIndex] == '"')
					{
						currentPair->Next = new LinkedPair();
						currentPair = currentPair->Next;
						readingValue = false;
					}
					else currentPair->Value += content[currentIndex];
				}
			}
		}
	}

	return currentIndex;
}

int elementIntoPieces(HTML* root, string tag, list<string> element, int currentIndex, string content)
{
	LinkedPair* dict = new LinkedPair();
	currentIndex = getTagArgs(tag, currentIndex, content, dict);
	bool readingVar;
	string add, current;

	for (string ePiece : element)
	{
		if (ePiece[0] == VAR_INDICATOR && ePiece[ePiece.size() - 1] == VAR_INDICATOR)
		{
			add = dict->GetValue(ePiece);
			readingVar = false;
			current = "";
			
			for (int i = 0; i < add.size(); i++)
			{
				if (add[i] == SLASH && add.size() - 1 > i && add[i + 1])
				{
					i++;
					current += add[i];
				}
				else if (add[i] == VAR_INDICATOR)
				{
					if (!readingVar)
					{
						root->Pieces.push_back(current);

						current = VAR_INDICATOR;
					}
					else
					{
						root->Pieces.push_back(current + VAR_INDICATOR);

						current = "";
					}

					readingVar = !readingVar;
				}
				else current += add[i];
			}

			if (current != "") root->Pieces.push_back(current);
		}
		else root->Pieces.push_back(ePiece);
	}

	return currentIndex;
}

void pushNonExisting(list<string>* a, list<string>* b)
{
	bool found;

	for (string add : *b)
	{
		found = false;

		for (string check : *a)
		{
			if (check == add)
			{
				found = true;

				break;
			}
		}

		if (!found) a->push_back(add);
	}
}

HTML::HTML(const char* id, string content)
{
	ID = id;
	auto titleIndex = -1;
	char reading = C_NULL;
	string current;
	LinkedPair* tagDict = new LinkedPair();
	LinkedPair* currentTag = tagDict;

	for (int i = 0; i < content.size(); i++)
	{
		if (content[i] == SLASH && content.size() - 1 > i)
		{
			if (content[i + 1] == SLASH || content[i + 1] == VAR_INDICATOR || content[i + 1] == STATEMENT_INDICATOR)
			{
				i++;
				current += content[i];
			}
		}
		else if (content[i] == VAR_INDICATOR || content[i] == STATEMENT_INDICATOR)
		{
			if (reading == C_NULL)
			{
				Pieces.push_back(current);

				current = reading = content[i];
			}
			else
			{
				Pieces.push_back(current + content[i]);

				current = "";
				reading = C_NULL;
			}
		}
		else if (reading == C_NULL && content[i] == '<' && content.size() - 1 > i && content[i + 1] != '/')
		{
			string tag = getTag(i + 1, content);
			HTML* element = tag.size() > 0 ? getHTML(tag.c_str(), nullptr, Server::Get()->Elements, false) : nullptr;

			if (element != nullptr)
			{
				Pieces.push_back(current);

				if (tag == "title") titleIndex = Pieces.size();
				else if (tagDict->GetValue(tag) == KEY_NOT_FOUND)
				{
					if (!element->Import.empty()) pushNonExisting(&Import, &element->Import);
					
					currentTag->Key = tag;
					currentTag->Next = new LinkedPair();
					currentTag = currentTag->Next;
				}

				current = "";
				i = elementIntoPieces(this, tag, element->Pieces, i + tag.size() + 1, content);
			}
			else 
			{
				current += content.substr(i, 1 + tag.size());
				i += tag.size();
			}
		}
		else current += content[i];
	}

	if (current != "") Pieces.push_back(current);

	if (titleIndex != -1)
	{
		auto it = Pieces.begin();

		advance(it, titleIndex);

		for (string link : Import) Pieces.insert(it, link);
	}
}

void HTML::AddCSS(string css)
{
	if (css.length() > 0) Import.push_back("<link rel=\"stylesheet\" href=\"" + css + "\" />");
}

void HTML::AddJS(string js)
{
	if (js.length() > 0) Import.push_back("<script rel=\"text/javascript\" src=\"" + js + "\" defer></script>");
}

HTML* getHTML(const char* id, const char* filePath, vector<HTML*> list, bool createIfNotExists)
{
	for (HTML* html : list)
	{
		if (strcmp(html->ID, id) == 0) return html;
	}

	if (createIfNotExists)
	{
		HTML* result = new HTML(id, getBody(filePath));

		list.push_back(result);

		return result;
	}

	return nullptr;
}
