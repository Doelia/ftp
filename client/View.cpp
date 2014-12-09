#include "View.h"

View* View::instance = NULL;

View* View::getInstance() {
	return View::instance;
}

void View::init() {
	cout << "Initialisation du View..." << endl;
	View::instance = new View();
}

View::View() {

	string fileKey = "./sem_disp";
	if (!FileManager::getInstance()->exists(fileKey)) {
		FileManager::getInstance()->createFile(fileKey);
	}

	key_t key = ftok("./sem_disp", 10);
	if (key == -1) {
		perror("Problème lors de la réservation de la clé");
		exit(0);
	}

	this->semid = semget(key, 2, IPC_CREAT | 0666);
	if (semid == -1) {
		perror("Problème lors du semget()");
		exit(0);
	}

	semctl(semid, 0, SETVAL, 0);
}

void View::refreshView(char* s, int size) {
	int fd = open("logs.txt", O_CREAT | O_WRONLY, 777);
	write(fd, s, size);

	struct sembuf op;
	op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

void View::openView() {
	if (fork()) {
		execl("xterm", "./a.out", "", "", "", (char *) 0);
	}
}
