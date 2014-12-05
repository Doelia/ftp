#include "ThreadManager.h"

ThreadManager* ThreadManager::instance = NULL;

ThreadManager* ThreadManager::getInstance() {
	return ThreadManager::instance;
}

void ThreadManager::init() {
	ThreadManager::instance = new ThreadManager();
}

ThreadManager::ThreadManager() {
	this->threads = new vector<pthread_t*>();
}

void ThreadManager::add(pthread_t* t) {
	this->display("Un thread a été ajouté");
	this->threads->push_back(t);
}

void ThreadManager::display(string s) {
	cout << "[ThreadManager] " << s << endl;
}

void ThreadManager::joinAll() {
	int n = 0;

	while (n != this->threads->size()) {
        this->display("Attente d'un thread...");
		pthread_join(*this->threads->at(n), NULL);
		n++;
	}

	
}

