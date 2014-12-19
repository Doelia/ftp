#ifndef CLIENT_CONNEXION
#define CLIENT_CONNEXION

#include "header-serveur.h"

class ClientConnexion : public Connexion {

private:
	void onPaquet(char*, int);
	void onPaquet_get(string);
	void onPaquet_fileData(Packet* p);

public:
	ClientConnexion(int);
	void onPaquet_put(string, int);

};

#endif
