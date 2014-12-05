#include "ServerManager.h"

ServerManager* ServerManager::instance = NULL;

ServerManager* ServerManager::getInstance() {
	return ServerManager::instance;
}

ServerManager* ServerManager::init() {
	ServerManager::instance = new ServerManager();
	return ServerManager::instance;
}

void ServerManager::acceptClients() {
	int sockClient;
	sockaddr_in addrClient;
	socklen_t size = sizeof(addrClient);

	cout << "En attente de connexion..." << endl;
	while ((sockClient = accept(sock, (sockaddr*)&addrClient, &size))) {
		cout << "Un client se connecte avec la socket " << sockClient << " de " << inet_ntoa(addrClient.sin_addr) << ": " << htons(addrClient.sin_port) << endl;
		ClientConnexion* cc = new ClientConnexion(sockClient);
		cc->start_listenMessages();
		cout << "En attente de connexion..." << endl;
	}
}

int ServerManager::startServer() {
	this->sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock != -1) {

		struct sockaddr_in addr;
		addr.sin_addr.s_addr	= htonl(INADDR_ANY);
		addr.sin_family			= AF_INET;
	 	addr.sin_port			= htons(PORT_SERVER);
	 	
		if (::bind(sock, (sockaddr*)&addr, sizeof(addr)) != -1) {
			cout << "Bind OK" << endl;

			if (listen(sock, SERVER_LIMIT_CLIENTS) != -1) {
				cout << "listen() sur le port " << PORT_SERVER << " OK" << endl;
				return 0;
			}
			else {
				cout << "Impossible d'écouter sur le port " << PORT_SERVER << endl;
				return 1;
			}
		}
		else {
			cout << "Bind de la socket sur le port " << PORT_SERVER << " impossible" << endl;
			return 2;
		}
	}
	else {
		cout << "Erreur creation de socket" << endl;
		return 3;
	}

	return 4;  
}


