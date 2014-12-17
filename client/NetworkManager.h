#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "header-client.h"

class NetworkManager : public Connexion {

public:
	static NetworkManager* getInstance();
	static NetworkManager* init();

	bool connectToServer(string, int);
	void sendGetFile(string);
	void sendPutFile(string, int);

private:
	static NetworkManager* instance;
	
	void onPaquet(char*, int);
	void onPaquet_message(string);
	void onPaquet_fileData(Packet* p);
	void onPaquet_fileHeader(string, int);

};

#endif
