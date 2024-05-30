#include "utils.hpp"
#include <fstream>
#include <filesystem>

string statusText(int status)
{
	switch (status)
	{
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 103: return "Early Hints";
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";
		case 226: return "IM Used";
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Content Too Large";
		case 414: return "URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Content";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 425: return "Too Early";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 451: return "Unavailable For Legal Reasons";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";
		default: return "";
	}
}

string getContentType(const char* filePath)
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

string getBody(const char* filePath)
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

string buildResponse(int status, string contentType, string content)
{
	return "HTTP/1.1 " + to_string(status) + " " + statusText(status) + "\r\nContent-Type: " + contentType +
		"\r\nContent-Length: " + to_string(content.size()) + "\r\nConnection: Closed\r\n\r\n" + content;
}
