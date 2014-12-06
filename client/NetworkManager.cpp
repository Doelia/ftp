#include "NetworkManager.h"

NetworkManager* NetworkManager::instance = NULL;

NetworkManager* NetworkManager::getInstance() {
	return NetworkManager::instance;
}

NetworkManager* NetworkManager::init() {
	NetworkManager::instance = new NetworkManager();
	return NetworkManager::instance;
}

bool NetworkManager::connectToServer(string ip, int port) {
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (this->sock != -1) {
        
        struct sockaddr_in addr;
        addr.sin_addr.s_addr    = inet_addr(ip.c_str());
        addr.sin_family         = AF_INET;
        addr.sin_port           = htons(port);
        
       cout << "Connexion à " << ip << ":" << port << "..." << endl;
        if (::connect(this->sock, (sockaddr*)&addr, sizeof(addr)) != -1) {
            cout << "Connecté au seveur !\n";
            return true;
        }
        else {
            cout << "Impossible de se connecter au serveur.\n";
        }
    }
    else {
        cout << "Erreur creation de socket\n";
    }

    return false;
}

void* networkManager_listenMessages(void *c) {
	NetworkManager::getInstance()->listenMessages();
	return NULL;
}

void NetworkManager::start_listenMessages() {
	pthread_t* id = (pthread_t*) malloc(sizeof(int)*2);
	pthread_create(id, NULL, networkManager_listenMessages, NULL);
	ThreadManager::getInstance()->add(id);
}

int cpt =0;

void NetworkManager::listenMessages() {
	char* buff;
	initBuffer(&buff, MAX_SIZE_PAQUETS);
	int retour;
	while ((retour = recv(this->sock, buff, MAX_SIZE_PAQUETS, 0)) > 0) {
		cpt++;
		cout << "packet" << cpt << ". ";
		if (buff[0] == '\0') {
			cout << "error au paquet " << cpt << endl;
		}
		this->onPaquet(buff);
		initBuffer(&buff, MAX_SIZE_PAQUETS);
	}
    cout << "Fin d'attende de message." << endl;
}

void NetworkManager::onPaquet(char* paquet) {

	Packet* p = new Packet(paquet);

	//cout << "Paquet reçu du serveur : ";
	//p->display();
	//Packet::displayPacket(paquet, p->getSizePacket());

	if (p->getId().compare("MSG") == 0) {
		this->onPaquet_message(p->getArgument());
		return;
	}

	if (p->getId().compare("RGE") == 0) { // Réponse du GET
		if (p->getArgument().compare("1") == 0) {
			cout << "Fichier trouvé. Le transfert va démarrer." << endl;
		} else {
			cout << "Fichier introuvable. Veuillez vérifier le nom entré puis réésayez." << endl;
		}
		Shell::getInstance()->unlockShell();
		return;
	}

	if (p->getId().compare("FID") == 0) { // File Head
		string nameFile = p->getArgument();
		int sizeString = p->getSizeData();
		cout << "reçu header file" << nameFile << " " << sizeString << endl;
		this->onPaquet_fileHeader("out_"+nameFile, sizeString);
		return;
	}

	if (p->getId().compare("FDA") == 0) { // Fide Data
		this->onPaquet_fileData(p);
		return;
	}

	cout << "Erreur réseau. Paquet non reconnu : " << paquet << endl;
	p->display();
}

void NetworkManager::onPaquet_message(string message) {
	// TODO
}


void NetworkManager::onPaquet_fileHeader(string nameFile, int sizeFile) {
	if (FileReceiver::getInstance()->prepareTransfert(nameFile, sizeFile)) {
		
	} else {
		cout << "Erreur sur FileReceiver::getInstance()->prepareTransfert()" << endl;
	}
}

void NetworkManager::onPaquet_fileData(Packet* p) {
	if (!FileReceiver::getInstance()->recvData(p)) {
		cout << "Erreur sur FileReceiver::getInstance()->recvData()" << endl;
	}
}

bool NetworkManager::sendPaquet(Packet* p) {
	char* buffer = p->constructPacket();
	//cout << "Paquet à envoyer = " << buffer << endl;
	int sock_err = send(this->sock, buffer, MAX_SIZE_PAQUETS, 0);
	return true;
}

void NetworkManager::sendGetFile(string namefile) {
	Packet* p = new Packet("GET", namefile, 0, NULL);
	this->sendPaquet(p);
}




