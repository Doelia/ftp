#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include "header-commun.h"
#include "Packet.h"

using namespace std;

class FileReceiver {

public:
	static FileReceiver* getInstance();
	static void init(ProgressNotifier*);

	FileReceiver(ProgressNotifier*);

	int prepareTransfert(string, int);
	int recvData(Packet* p);
	
private:
	static FileReceiver* instance;

	vector<int>* descs; // Descripteurs des fichiers en cours de transferts
	vector<double>* sizeFiles;
	vector<string>* listFiles;
	vector<double>* transfered;
	ProgressNotifier* pn;

	int getKeyFromNameFile(string);
	int nextKey;

};

#endif
