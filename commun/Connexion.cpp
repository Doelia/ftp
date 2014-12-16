#include "Connexion.h"

void Connexion::start_listenMessages() {
	pthread_t* id = (pthread_t*) malloc(sizeof(int)*2);
	pthread_create(id, NULL, connexion_listenMessages, (void*) this);
	ThreadManager::getInstance()->add(id);
}


void* connexion_listenMessages(void *c) {
	Connexion* instance = (Connexion*) c;
	instance->listenMessages();
	return NULL;
}

void Connexion::listenMessages() {
	char* buff;
	initBuffer(&buff, MAX_SIZE_PAQUETS);
	int retour;
	while ((retour = recv(this->sock, buff, MAX_SIZE_PAQUETS, 0)) > 0) {
		this->onPaquet(buff, retour);
		initBuffer(&buff, MAX_SIZE_PAQUETS, false);
	}
    cout << "Fin d'attende de message." << endl;
}

bool Connexion::sendPaquet(Packet* p) {
	char* buffer = p->constructPacket();
	int sock_err = send(this->sock, buffer, MAX_SIZE_PAQUETS, 0);
	free(buffer);

	if (sock_err < 0) {
		perror("Erreur lors de l'envoi du paquet");
		return false;
	} else {
		return true;
	}
}

