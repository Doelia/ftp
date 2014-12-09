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


void NetworkManager::onPaquet(char* paquet, int size) {

	Packet* p = new Packet(paquet, size);

	if (p->getId().compare("MSG") == 0) {
		this->onPaquet_message(p->getArgument());
	}

	else if (p->getId().compare("RGE") == 0) { // Réponse du GET
		if (p->getArgument().compare("1") == 0) {
			cout << "Fichier trouvé. Le transfert va démarrer." << endl;
		} else {
			cout << "Fichier introuvable. Veuillez vérifier le nom entré puis réésayez." << endl;
		}
		Shell::getInstance()->unlockShell();
	}

	else if (p->getId().compare("FID") == 0) { // File Head
		string nameFile = p->getArgument();
		int sizeString = p->getSizeData();
		cout << "reçu header file : " << nameFile << " " << sizeString << endl;
		this->onPaquet_fileHeader("out_"+nameFile, sizeString);
	}

	else if (p->getId().compare("FDA") == 0) { // Fide Data
		this->onPaquet_fileData(p);
	}
	
	else {
		cout << "Erreur réseau. Paquet non reconnu : " << paquet << endl;
		p->display();
	}
	
	p->deleteFromMemory();
	free(p);
}

void NetworkManager::onPaquet_message(string message) {
	
}

void NetworkManager::onPaquet_fileHeader(string nameFile, int sizeFile) {
	if (FileReceiver::getInstance()->prepareTransfert(nameFile, sizeFile) > 0) {

	} else {
		cout << "Erreur sur FileReceiver::getInstance()->prepareTransfert()" << endl;
	}
}

void NetworkManager::onPaquet_fileData(Packet* p) {
	if (!FileReceiver::getInstance()->recvData(p)) {
		cout << "Erreur sur FileReceiver::getInstance()->recvData()" << endl;
	}
}

void NetworkManager::sendGetFile(string namefile) {
	Packet* p = new Packet("GET", namefile, 0, NULL);
	this->sendPaquet(p);
	p->deleteFromMemory();
	free(p);
}


