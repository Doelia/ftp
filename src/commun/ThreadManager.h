#ifndef THREADMANAGER
#define THREADMANAGER

#include "header-commun.h"

using namespace std;

class ThreadManager {
	public:
		static ThreadManager* getInstance();
		static void init();
		ThreadManager();
		void joinAll();
		void display(string s);
		void add(pthread_t* t);
	private:
		static ThreadManager* instance;
		vector<pthread_t*>* threads;
};

#endif