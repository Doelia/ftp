#ifndef CONNEXION_H
#define CONNEXION_H

class Connexion;

#include "header-commun.h"

void* connexion_listenMessages(void *c);

class Connexion {

public:
	virtual void start_listenMessages();
	virtual void listenMessages();
	virtual bool sendPaquet(Packet*);

protected:
	int sock;

	virtual void onPaquet(char*, int) = 0;

};

#endif
