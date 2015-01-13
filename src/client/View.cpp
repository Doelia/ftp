#include "View.h"

View* View::instance = NULL;

View* View::getInstance() {
	return View::instance;
}

View* View::init() {
	cout << "Initialisation du View..." << endl;
	View::instance = new View();
	return View::instance;
}

View::View() {

	char fileKey[21];
	sprintf(fileKey, "/tmp/sem_view%d", getpid());

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

	this->listTranfers = new vector <FileInTransfert>();
}

void View::refreshView() {
	char nameFile[21];
	sprintf(nameFile, "/tmp/logs_%d.txt", getpid());
	ofstream fichier(nameFile, ios::out | ios::trunc);
	fichier << out.rdbuf();
	out.str("");

	struct sembuf op;
	op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

void* start_view(void *c) {
	char cmd[21];
	sprintf(cmd, "xterm -e \"./viewer %d\"", getpid());

	system(cmd);
	exit(0);
	return NULL;
}

void View::openView() {
	this->constructFrame();
	this->refreshView();
	if (!access ("viewer", X_OK)) {
		pthread_t* id = (pthread_t*) malloc(sizeof(int)*2);
		pthread_create(id, NULL, start_view, NULL);
		ThreadManager::getInstance()->add(id);
	} else {
		perror("Erreur lors de l'ouverture de l'executable 'viewer' : ");
		stop();
	}
}

void View::close() {
}

void View::onFileStart(string f, int size, int type) {
	FileInTransfert s;
	s.pourcent = 0; 
	s.nameFile = f;
	s.isFinished = false;
	s.type = type;
	this->listTranfers->push_back(s);
}

int View::getIndexOfFile(string nameFile) {
	for (int i = 0; i < this->listTranfers->size(); ++i) {
		if (this->listTranfers->at(i).nameFile.compare(nameFile) == 0) {
			return i;
		}
	}
	return -1;
}

void View::onFileProgress(string nameFile, int pourcent, int type) {
	int i = this->getIndexOfFile(nameFile);
	if (i >= 0) {
		this->listTranfers->at(i).pourcent = pourcent;
		this->constructFrame();
		this->refreshView();
	}
}

void View::onFileEnd(string nameFile, int type) {
	int i = this->getIndexOfFile(nameFile);
	if (i >= 0) {
		this->listTranfers->at(i).isFinished = true;
		this->constructFrame();
		this->refreshView();
	}
}

void View::constructFrame() {
	out.str("");
	out << "Fichiers en cours de réceptions" << endl;

	for (int i=0; i < this->listTranfers->size(); i++) {
		FileInTransfert s = this->listTranfers->at(i);
		if (s.type == 2) {
			if (s.isFinished) {
			out << s.nameFile << " - Terminé!" << endl;
			} else {
				out << s.nameFile << " " << this->getProgressBar(s.pourcent) << " " << s.pourcent << "%" << endl;
			}
		}
	}
	
	out << "----------------------------------------------------" << endl;

	out << "Fichiers en cours d'envoi" << endl;

	for (int i=0; i < this->listTranfers->size(); i++) {
		FileInTransfert s = this->listTranfers->at(i);
		if (s.type == 1) {
			if (s.isFinished) {
			out << s.nameFile << " - Terminé!" << endl;
			} else {
				out << s.nameFile << " " << this->getProgressBar(s.pourcent) << " " << s.pourcent << "%" << endl;
			}
		}
	}
	
	out << "----------------------------------------------------" << endl;
}
			
void View::writeFrame() {
	
}


string View::getProgressBar(int pourcent) {

	int nbrBars = 30;
	string s = "[";
	int nbrBarsActives = pourcent * nbrBars / 100;

	for (int i = 0; i < nbrBarsActives; ++i) {
		s += "=";
	}
	s += ">";

	for (int i = nbrBarsActives; i < nbrBars; ++i) {
		s += " ";
	}
	s += "]";

	return s;
}
