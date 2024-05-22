#include "response.hpp"

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

std::string render(const char* filePath, const char* tKeys[], const char* tValues[], int tSize)
{
	std::string output = "";
	
	return output;
}
