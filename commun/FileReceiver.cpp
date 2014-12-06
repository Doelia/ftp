#include "FileReceiver.h"

FileReceiver* FileReceiver::instance = NULL;

FileReceiver* FileReceiver::getInstance() {
	return FileReceiver::instance;
}

void FileReceiver::init(ProgressNotifier* p_n) {
	FileReceiver::instance = new FileReceiver(p_n);
}

FileReceiver::FileReceiver(ProgressNotifier* p_pn) {
	this->pn = p_pn;
}

int FileReceiver::prepareTransfert(string nameFile, int size) {
	int fd = open(nameFile.c_str(), O_CREAT | O_WRONLY, 777);
	if (fd) {
		string idFile = nameFile;
		this->descs[idFile] = fd;
		this->sizeFiles[idFile] = (double) size;
		this->transfered[idFile] = 0.0;
		if (this->pn != NULL) {
			this->pn->onFileStart(nameFile);
		}
	}
	return fd;
}

int FileReceiver::recvData(Packet* p) {
	string nameFile = p->getArgument();
	int lengthInt = p->getSizeData();
	char* datas = p->getDatas();
	string idFile = nameFile;

	int desc = descs[idFile];
	int totalSize = sizeFiles[idFile];
	
	if ((write(desc, datas, lengthInt))) {
		this->transfered[idFile] += (double) lengthInt;

		if (this->pn != NULL) {
			int pourcent = this->transfered[idFile] / totalSize * 100;
			this->pn->onFileProgress(nameFile, pourcent);
		}

		return 1;
	}

	return 0;

}
