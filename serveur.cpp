#include "serveur/header-serveur.h"

void stop() {
	cout << "Fermerture forcée." << endl;
	exit(0);
}

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


	cout << "Fermerture du serveur..." << endl;
	ThreadManager::getInstance()->joinAll();
}

