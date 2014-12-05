#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "header-client.h"

void* networkManager_listenMessages(void *c);

class NetworkManager {

public:
	static NetworkManager* getInstance();
	static NetworkManager* init();

	bool connectToServer(string, int);
	void start_listenMessages();
	void listenMessages();
	void sendGetFile(string);

private:
	static NetworkManager* instance;
	
	int sock;

	void onPaquet(string);
	void onPaquet_message(string);
	void onPaquet_fileData(string, string, int);
	void onPaquet_fileHeader(string, int);
	bool sendPaquet(string);

};

#endif
