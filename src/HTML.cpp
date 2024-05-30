#include "HTML.hpp"
#include "http/utils.hpp"
#include "LinkedPair.hpp"
#include "Server.hpp"
#include <string.h>

string getTag(int start, string content, int length)
{
	string result = "";

	for (int i = start; i < length; i++)
	{
		if (content[i] == ' ' || content[i] == '>' || content[i] == '/') break;
		else result += content[i];
	}

	return result;
}

int getTagArgs(string tag, int tagLength, int currentIndex, string content, int length, LinkedPair* dict)
{
	LinkedPair* currentPair = dict;
	bool readingContent, readingValue;

	for (; currentIndex < length; currentIndex++)
	{
		if (readingContent)
		{
			if (content[currentIndex] == '<' && length - 1 > currentIndex + tagLength + 1
				&& content.substr(currentIndex, tagLength + 3) == "</" + tag + ">")
			{
				currentIndex += tagLength + 3;

				break;
			}

			currentPair->Value += content[currentIndex];
		}
		else
		{
			if (content[currentIndex] == '/' && length - 1 > currentIndex && content[currentIndex + 1] == '>')
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
					if (length - 1 > currentIndex && content[currentIndex] == '"')
					{
						currentPair->Value += content[currentIndex + 1];
						currentIndex++;
					}
				}
				else
				{
					if (content[currentIndex] == SLASH && length - 1 > currentIndex && content[currentIndex + 1] == '"')
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

int elementIntoPieces(HTML* root, string tag, int tagLength, vector<string> element, int currentIndex, string content, int length)
{
	LinkedPair* dict = new LinkedPair();
	currentIndex = getTagArgs(tag, tagLength, currentIndex, content, length, dict);

	for (string ePiece : element)
	{
		if (ePiece[0] == VAR_INDICATOR && ePiece[ePiece.size() - 1] == VAR_INDICATOR) root->Pieces.push_back(dict->GetValue(ePiece));
		else root->Pieces.push_back(ePiece);
	}

	return currentIndex;
}

HTML::HTML(const char* id, string content)
{
	ID = id;
	int length = content.size();
	char reading = C_NULL;
	string current;

	for (int i = 0; i < length; i++)
	{
		if (content[i] == SLASH && length - 1 > i)
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
		else if (reading == C_NULL && content[i] == '<' && length - 1 > i && content[i + 1] != '/')
		{
			string tag = getTag(i + 1, content, length);
			int tagLength = tag.size();
			HTML* element = tagLength > 0 ? getHTML(tag.c_str(), nullptr, Server::Get()->Elements, false) : nullptr;

			if (element != nullptr)
			{
				Pieces.push_back(current);

				current = C_NULL;
				i = elementIntoPieces(this, tag, tagLength, element->Pieces, i + tagLength + 1, content, length);
			}
			else 
			{
				current += content.substr(i, 1 + tagLength);
				i += tagLength;
			}
		}
		else current += content[i];
	}

	if (current != "") Pieces.push_back(current);
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
