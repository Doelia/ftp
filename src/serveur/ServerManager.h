#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "header-serveur.h"

class ServerManager {

public:
	static ServerManager* getInstance();
	static ServerManager* init();

	int startServer();
	void acceptClients();

private:
	int sock;
	static ServerManager* instance;
};

#endif
