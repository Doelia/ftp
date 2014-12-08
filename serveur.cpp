#include "serveur/header-serveur.h"

int main() {

	FileManager::init();
	ThreadManager::init();
	ServerManager::init();
	FileReceiver::init(NULL);

	ServerManager* server = ServerManager::getInstance();
	int error = 0;
	if (!(error = server->startServer())) {
		server->acceptClients();
	}

	ThreadManager::getInstance()->joinAll();
}

