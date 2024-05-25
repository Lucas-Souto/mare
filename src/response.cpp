#include "response.hpp"
#include "server.hpp"
#include <string.h>

static const char VAR_INDICATOR = '$';

std::string getTag(int start, std::string content, int length)
{
	std::string result = "";
	
	for (int i = start; i < length; i++)
	{
		if (content[i] == ' ' || content[i] == '>' || content[i] == '/') break;
		else result += content[i];
	}

	return result;
}

HTML* getHTML(const char* id, const char* filePath, std::list<HTML*> list, bool createIfNotExists)
{
	HTML* result = nullptr;

	for (HTML* html : list)
	{
		if (strcmp(html->id, id) == 0)
		{
			result = html;

			break;
		}
	}

	if (result == nullptr && createIfNotExists)
	{
		result = new HTML(id, getBody(filePath));
		list.push_back(result);
	}

	return result;
}

HTML::HTML(const char* id, std::string content)
{
	this->id = id;
	int length = content.size();
	bool readingVar = false;
	std::string current = "";
	std::string readingTag = "";

	for (int i = 0; i < length; i++)
	{
		if (content[i] == '\\' && length - 1 > i)
		{
			if (content[i + 1] == VAR_INDICATOR)
			{
				i++;
				current += content[i];
			}
			else if (content[i + 1] == '\\')
			{
				current += content[i];
				i++;
			}
		}
		else if (content[i] == VAR_INDICATOR)
		{
			if (!readingVar)		
			{
				this->pieces.push_back(current);

				current = VAR_INDICATOR;
			}
			else
			{
				this->pieces.push_back(current + VAR_INDICATOR);

				current = "";
			}

			readingVar = !readingVar;
		}
		else if (readingTag != "")
		{
			if (content[i] == '/' && content[i + 1] == '>' && length - 1 > i)
			{
				current += content[i];
				current += content[i + 1];
				i++;
				readingTag = "";

				this->pieces.push_back(current);

				current = "";
			}
			else if (content[i] == '<' && content[i + 1] == '/' && length - 1 > i + readingTag.size() + 2)
			{
				current += content.substr(i, 3 + readingTag.size());
				i += 2 + readingTag.size();
				readingTag = "";

				this->pieces.push_back(current);

				current = "";
			}
			else current += content[i];
		}
		else if (!readingVar && content[i] == '<' && length - 1 > i && content[i + 1] != '/')
		{
			readingTag = getTag(i + 1, content, length);

			if (readingTag != "" && getHTML(readingTag.c_str(), nullptr, Server::get()->elements, false) != nullptr)
			{
				this->pieces.push_back(current);

				current = content.substr(i, 1 + readingTag.size());
				i += readingTag.size();
			}
			else
			{
				current += content.substr(i, 1 + readingTag.size());
				i += readingTag.size();
				readingTag = "";
			}
		}
		else current += content[i];
	}

	if (current != "") this->pieces.push_back(current);
}

CharDict::CharDict(int size)
{
	key = "";
	value = "";
	
	if (size > 1) next = new CharDict(size - 1);
	else next = nullptr;
}

void CharDict::set(std::string key, std::string value)
{
	this->key = key;
	this->value = value;
}

std::string findValue(std::string key, CharDict* first)
{
	if (key == first->key) return first->value;
	else if (first->next != nullptr) return findValue(key, first->next);
	
	return "$NIL$";
}

std::string getContentType(const char* filePath)
{
	std::filesystem::path path = filePath;
	std::string ext(path.extension());

	// Text
	if (ext == ".css") return "text/css";
	else if (ext == ".csv") return "text/csv";
	else if (ext == ".html" || ext == ".htm") return "text/html";
	else if (ext == ".ics") return "text/calendar";
	else if (ext == ".js") return "text/javascript";
	else if (ext == ".mjs") return "text/javascript";
	else if (ext == ".txt") return "text/plain";
	else if (ext == ".xml") return "application/xml";
	else if (ext == ".json") return "application/json";
	else if (ext == ".jsonld") return "application/ld+json";
	else if (ext == ".xhtml") return "application/xhtml+xml";
	// Image
	else if (ext == ".bmp") return "image/bmp";
	else if (ext == ".gif") return "image/gif";
	else if (ext == ".ico") return "image/vnd.microsoft.icon";
	else if (ext == ".jpeg" || ext == ".jpg") return "image/jpeg";
	else if (ext == ".png") return "image/png";
	else if (ext == ".svg") return "image/svg+xml";
	else if (ext == ".tif" || ext == ".tiff") return "image/tiff";
	else if (ext == ".webp") return "image/webp";
	// Video
	else if (ext == ".avi") return "video/x-msvideo";
	else if (ext == ".mp4") return "video/mp4";
	else if (ext == ".mpeg") return "video/mpeg";
	else if (ext == ".ogv") return "video/ogg";
	else if (ext == ".ts") return "video/mp2t";
	else if (ext == ".webm") return "video/webm";
	else if (ext == ".3gp") return "video/3gpp";
	else if (ext == ".3g2") return "video/3gpp2";
	// Audio
	else if (ext == ".aac") return "audio/aac";
	else if (ext == ".mid" || ext == ".midi") return "audio/midi audio/x-midi";
	else if (ext == ".mp3") return "audio/mpeg";
	else if (ext == ".oga") return "audio/ogg";
	else if (ext == ".opus") return "audio/opus";
	else if (ext == ".wav") return "audio/wav";
	else if (ext == ".weba") return "audio/webm";
	// Font
	else if (ext == ".otf") return "font/otf";
	else if (ext == ".ttf") return "font/ttf";
	else if (ext == ".woff") return "font/woff";
	else if (ext == ".woff2") return "font/woff2";
	// Applications
	else if (ext == ".abw") return "application/x-abiword";
	else if (ext == ".arc") return "application/x-freearc";
	else if (ext == ".azw") return "application/vnd.amazon.ebook";
	else if (ext == ".bin") return "application/octet-stream";
	else if (ext == ".bz") return "application/x-bzip";
	else if (ext == ".bz2") return "application/x-bzip2";
	else if (ext == ".csh") return "application/x-csh";
	else if (ext == ".doc") return "application/msword";
	else if (ext == ".docx") return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	else if (ext == ".eot") return "application/vnd.ms-fontobject";
	else if (ext == ".epub") return "application/epub+zip";
	else if (ext == ".gz") return "application/gzip";
	else if (ext == ".jar") return "application/java-archive";
	else if (ext == ".mpkg") return "application/vnd.apple.installer+xml";
	else if (ext == ".odp") return "application/vnd.oasis.opendocument.presentation";
	else if (ext == ".ods") return "application/vnd.oasis.opendocument.spreadsheet";
	else if (ext == ".odt") return "application/vnd.oasis.opendocument.text";
	else if (ext == ".ogx") return "application/ogg";
	else if (ext == ".pdf") return "application/pdf";
	else if (ext == ".php") return "application/x-httpd-php";
	else if (ext == ".ppt") return "application/vnd.ms-powerpoint";
	else if (ext == ".pptx") return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	else if (ext == ".rar") return "application/vnd.rar";
	else if (ext == ".rtf") return "application/rtf";
	else if (ext == ".sh") return "application/x-sh";
	else if (ext == ".swf") return "application/x-shockwave-flash";
	else if (ext == ".tar") return "application/x-tar";
	else if (ext == ".vsd") return "application/vnd.visio";
	else if (ext == ".xls") return "application/vnd.ms-excel";
	else if (ext == ".xlsx") return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	else if (ext == ".xul") return "application/vnd.mozilla.xul+xml";
	else if (ext == ".zip") return "application/zip";
	else if (ext == ".7z") return "application/x-7z-compressed";

	else return "text/plain";
}
std::string getBody(const char* filePath)
{
	std::string output = "";
	std::ifstream file(filePath);

	if (file.is_open())
	{
		output.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
	}

	return output;
}

std::string render(const char* filePath, CharDict* dict)
{
	std::string output = "";
	HTML* page = getHTML(filePath, filePath, Server::get()->pages, true);

	for (std::string piece : page->pieces)
	{
		if (piece[0] == VAR_INDICATOR && piece[piece.size() - 1] == VAR_INDICATOR) output += findValue(piece.substr(1, piece.size() - 2), dict);
		else output += piece;
	}

	return output;
}
