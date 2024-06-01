#ifndef SERVER_HPP
#define SERVER_HPP
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "http/Route.hpp"
#include "http/Request.hpp"
#include "HTML.hpp"
// SSL
#include <openssl/ssl.h>

extern "C"
{
	#include <lua.h>
}

using namespace std;

static const int DEFAULT_PORT = 8000;

class Server
{
public:
	lua_State* L;
	vector<Route*> Routes;
	vector<HTML*> Pages, Elements;
	string Page404 = "<h1>Error 404</h1><p>Content not found!</p>";

	bool IsRunning() { return running; }
	static Server* Get() { return Server::instance; }

	Server();

	void Init(int port, bool https);
	void StartListen(int backlog);
	void Close();
	void InitLua();

private:
	int sockfd;
	bool running, https;
	sockaddr_in sockaddr;
	SSL_CTX* ctx;
	static Server* instance;

	void ListenClients();
	void ResponseTo(int connection, SSL* ssl, char (&buffer)[BUFFER_SIZE]);
};
#endif
