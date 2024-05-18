#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);// IPV4, TCP

	if (sockfd == -1) return -1;

	// Ouvir a porta X, em qualquer endereço
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(8000);// Número para Network Byte Order
	
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) return -2;

	if (listen(sockfd, 10) < 0) return -3;// Começa a ouvir, carregando até 10 conexões por vez

	auto addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);

	if (connection < 0) return -4;

	char buffer[100];
	auto bytesRead = read(connection, buffer, 100);

	printf("A mensagem foi: %s", buffer);

	std::string response = "Opa!\n";

	send(connection, response.c_str(), response.size(), 0);
	close(connection);
	close(sockfd);
	/* lua_State* L = luaL_newstate();

	luaL_openlibs(L);
	printf("Lua:\n");
	luaL_dofile(L, "test.lua");
	lua_close(L); */

	return 0;
}
