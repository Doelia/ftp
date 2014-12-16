#ifndef VIEW_H
#define VIEW_H

#include "header-client.h"

struct FileInTransfert {
	int pourcent;
	string nameFile;
	int type; // 1 = DL, 2 = UP
};

class View : public ProgressNotifier {

public:
	static View* getInstance();
	static View* init();

	View();
	void openView();

	virtual void onFileStart(string);
	virtual void onFileProgress(string nameFile, int pourcent);

private:
	static View* instance;
	int semid;
	vector<FileInTransfert>* listTranfers;

	stringstream out;

	void refreshView();
	void writeFrame();
	void constructFrame();
	int getIndexOfFile(string);
	string getProgressBar(int);

	
};

#endif
