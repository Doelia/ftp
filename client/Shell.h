#ifndef SHELL_H
#define SHELL_H

#include "header-client.h"

class Shell {

public:
	static Shell* getInstance();
	static void init();

	Shell();

	void welcomeMsg();
	void printPrompt();
	void promptCommand();

	void waitReponse();
	void unlockShell();

private:
	int semid;
	static Shell* instance;
	void traiterCommand(string);

};

#endif
