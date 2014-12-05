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


void NetworkManager::listenMessages() {
	char* buff;
	initBuffer(&buff, MAX_SIZE_PAQUETS);
	int retour;
	while ((retour = recv(this->sock, buff, MAX_SIZE_PAQUETS, 0)) > 0) {
		this->onPaquet(buff);
		initBuffer(&buff, MAX_SIZE_PAQUETS);
	}
    cout << "Fin d'attende de message." << endl;
}

void NetworkManager::onPaquet(string paquet) {
	//cout << "Paquet reçu du serveur : '" << paquet << "'" << endl;
	vector<string> parts = split(paquet, DELI());
	if (parts.size() == 0) {
		cout << "Erreur, paquet vide" << endl;
		return;
	}

	if (parts.at(0).compare("MSG") == 0) {
		this->onPaquet_message(parts.at(1));
		return;
	}

	if (parts.at(0).compare("REP_GET") == 0) {
		string reponse = parts.at(1);
		if (reponse.compare("1") == 0) {
			cout << "Fichier trouvé. Le transfert va démarrer." << endl;
		} else {
			cout << "Fichier introuvable. Veuillez vérifier le nom entré puis réésayez." << endl;
		}
		Shell::getInstance()->unlockShell();
		return;
	}

	if (parts.at(0).compare("FILE_HEAD") == 0) {
		string nameFile = parts.at(1);
		string sizeString = parts.at(2);
		cout << "reçu header file" << nameFile << " " << sizeString << endl;
		this->onPaquet_fileHeader("out_"+nameFile, atoi(sizeString.c_str()));
		return;
	}

	if (parts.at(0).compare("FILE_DATA") == 0) {
		string nameFile = parts.at(1);
		string length = parts.at(2);
		int lengthInt =  atoi(length.c_str());
		string data = parts.at(3);
		const char* datas = data.substr(0, lengthInt).c_str();
		this->onPaquet_fileData(nameFile, datas, atoi(length.c_str()));
		return;
	}

	cout << "Erreur réseau. Paquet non reconnu : " << paquet << endl;
}

void NetworkManager::onPaquet_message(string message) {
	//cout << "Message reçu du serveur : " << message << endl;
}

int fd;

void NetworkManager::onPaquet_fileHeader(string nameFile, int sizeFile) {
	fd = open(nameFile.c_str(), O_CREAT | O_WRONLY, 777);
}

void NetworkManager::onPaquet_fileData(string nameFile, const char* data, int length) {
	//cout << "paquet de longeur " << length << " reçu : " << data << endl;
	write(fd, data, length);
}

bool NetworkManager::sendPaquet(string paquet) {
	//cout << "Tentative d'envoi du paquet '" << paquet << "'" << endl;
	char* buffer;
	initBuffer(&buffer, MAX_SIZE_PAQUETS);
	int sizePaquet = paquet.size();

	if (sizePaquet >= MAX_SIZE_PAQUETS) {
		cout << "Erreur. Paquet trop gros (" << sizePaquet << "/" << MAX_SIZE_PAQUETS << endl;
		return false;
	}

	for (int i=0; i < sizePaquet; i++) {
		buffer[i] = paquet.at(i);
	}

	int sock_err = send(this->sock, buffer, MAX_SIZE_PAQUETS, 0);
	return true;
}

void NetworkManager::sendGetFile(string namefile) {
	this->sendPaquet("GET"+DELI()+namefile);
}

