#include "LogsTransfers.h"

LogsTransfers::LogsTransfers() {

}

void LogsTransfers::onFileStart(string name, int size, int type) {
	if (type == 2)
		cout << "La reception du fichier " << name << " commence. Taille=" << size << endl;
	else
		cout << "Fin d'envoi du fichier" << name << endl;
}

void LogsTransfers::onFileProgress(string name, int pourcent, int type) {
	if (type == 2) 
		cout << pourcent << "% reçu du fichier " << name << endl;
}


void LogsTransfers::onFileEnd(string name, int type) {
	if (type == 2) {
		cout << "Reception du fichier " << name << " terminé." << endl;
	} else {
		cout << "Envoi du fichier " << name << " au client..." << endl;
	}
}

