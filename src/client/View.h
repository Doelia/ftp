#ifndef VIEW_H
#define VIEW_H

#include "header-client.h"

void* start_view(void *c);


struct FileInTransfert {
	int pourcent;
	string nameFile;
	int type; // 2 = DL, 1 = UP
	bool isFinished;
};

class View : public ProgressNotifier {

public:
	static View* getInstance();
	static View* init();

	View();
	void openView();
	void close();

	virtual void onFileStart(string, int, int);
	virtual void onFileProgress(string nameFile, int pourcent, int);
	virtual void onFileEnd(string, int);

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
