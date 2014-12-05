#ifndef CLIENT_CONNEXION
#define CLIENT_CONNEXION

#include "header-serveur.h"

void* client_listeMessages(void* c);

class ClientConnexion {

private:

	static string SEP;

	int sock;
	void onPaquet(char*);
	void onPaquet_get(string);

	// Tests
	void sendPartFile(string, char*, int);
	void startSendFile(string);

public:
	ClientConnexion(int);
	void listenMessages();
	void start_listenMessages();
	int sendPaquet(Packet*);

};

#endif
