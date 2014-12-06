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
	
	unordered_map <string, int> descs; // Descripteurs des fichiers en cours de transferts
	unordered_map <string, double> sizeFiles; // Descripteurs des fichiers en cours de transferts
	unordered_map <string, double> transfered; // Descripteurs des fichiers en cours de transferts
	ProgressNotifier* pn;

};

#endif
