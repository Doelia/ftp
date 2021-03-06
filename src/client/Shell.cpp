#include "Shell.h"

Shell* Shell::instance = NULL;

Shell* Shell::getInstance() {
	return Shell::instance;
}

void Shell::init() {
	cout << "Initialisation du Shell..." << endl;
	Shell::instance = new Shell();
}

Shell::Shell() {

	char fileKey[21];
	sprintf(fileKey, "/tmp/sem%d", getpid());

	if (!FileManager::getInstance()->fileExists(fileKey)) {
		FileManager::getInstance()->createFile(fileKey);
	}

	key_t key = ftok(fileKey, 10);
	if (key == -1) {
		perror("Problème lors de la réservation de la clé");
		stop();
	}

	this->semid = semget(key, 2, IPC_CREAT | 0666);
	if (semid == -1) {
		perror("Problème lors du semget()");
		stop();
	}

	semctl(semid, 0, SETVAL, 0);
}

void Shell::waitReponse() {
	cout << "En attente du serveur..." << endl;
	struct sembuf op;
	op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

void Shell::unlockShell() {
	struct sembuf op;
	op.sem_num = 0;
    op.sem_op = 1; 
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

void Shell::welcomeMsg() {
	cout << "Bienvenue sur le serveur" << endl;
}

void Shell::printPrompt() {
	cout << endl << "Entrez une commande. Tapez 'help' pour avoir de l'aide, ou 'stop' pour quitter" << endl;
	cout << "> ";
}

void Shell::promptCommand() {

	string buff;

	this->printPrompt();
	getline(cin, buff);

	while (buff.compare("stop") != 0) {
		traiterCommand(buff);

		this->printPrompt();
		getline(cin, buff);
	}

	stop();
}

void Shell::printHelp() {
	cout << "Commandes disponibles :" << endl;
	cout << "- get <nom du fichier> Permet de récupérer un fichier stocké sur le serveur" << endl;
	cout << "- put <nom du fichier> Permet de récupérer d'envoyer un fichier sur le serveur" << endl;
	cout << "- ls Permet de lister les fichiers stockés sur le serveur" << endl;
	cout << "- stop : Ferme l'application" << endl;
}

void Shell::traiterCommand(string line) {

	vector<string> parts = split(line, " ");
	if (parts.size() < 1) {
		return;
	}

	string cmd = parts.at(0);

	if (cmd.compare("help") == 0) {
		this->printHelp();
		return;
	}

	if (cmd.compare("get") == 0) {

		if (parts.size() < 2) {
			cout << "Veuillez entrer un nom de fichier." << endl;
			return;
		}

		string nameFile = parts.at(1);

		if (FileManager::getInstance()->fileExistsInDir(nameFile)) {
			cout << "Le fichier" << nameFile << " existe déjà dans votre répértoire. Reception imposible." << endl;
			return;
		}

		NetworkManager::getInstance()->sendGetFile(nameFile);
		this->waitReponse();
		return;
	}

	if (cmd.compare("put") == 0) {

		if (parts.size() < 2) {
			cout << "Veuillez entrer un nom de fichier." << endl;
			return;
		}

		string nameFile = parts.at(1);
		if (FileManager::getInstance()->fileExistsInDir(nameFile)) {
			int size = FileManager::getInstance()->getSize(nameFile);
			NetworkManager::getInstance()->sendPutFile(nameFile, size);
			this->waitReponse();
		} else {
			cout << "Ce fichier est introuvable dans votre répértoire." << endl;
			return;
		}
	
		return;
	}


	cout << "Commande introuvable." << endl;
}
