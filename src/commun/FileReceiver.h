#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include "Packet.h"

using namespace std;

class FileReceiver {

public:
	static FileReceiver* getInstance();
	static void init(ProgressNotifier*);

	FileReceiver(ProgressNotifier*);

	int prepareTransfert(string, int);
	bool isInTransfert(string);
	int recvData(Packet* p);
	
private:
	static FileReceiver* instance;

	vector<string>* listFiles; // Liste des noms de fichiers, le I correspond au I dans les autres teableaux
	vector<int>* descs; // Descripteurs des fichiers en cours de transferts
	vector<double>* sizeFiles; // Taille des fichiers
	vector<double>* transfered; // Nombre d'octets transférés
	ProgressNotifier* pn;

	int getKeyFromNameFile(string);
	int nextKey;

};

#endif
