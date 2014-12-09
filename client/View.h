#ifndef VIEW_H
#define VIEW_H

#include "header-client.h"

class View {

public:
	static View* getInstance();
	static void init();

	View();
	void refreshView(char*, int);
	void openView();

private:
	static View* instance;
	int semid;
};

#endif
