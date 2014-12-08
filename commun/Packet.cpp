#include "Packet.h"

Packet::Packet(string p_id, string p_param, int p_sizeData, char* p_data) {

	//cout << "Création d'un paquet, ID=" << p_id << ", param=" << p_param << ", sizeData=" << p_sizeData << ", p_data=" << p_data << endl;

	initBuffer(&char_id, 4);
	initBuffer(&char_param, 21);

	for (int i=0; i < p_id.size(); i++) {
		this->char_id[i] = p_id[i];
	}

	for (int i=0; i < p_param.size(); i++) {
		this->char_param[i] = p_param[i];
	}

	this->sizeData = p_sizeData;

	if (p_data == NULL) {
		this->haveData = false;
		initBuffer(&data, 1);
	} else {
		this->haveData = true;
		this->data = p_data;
	}

}

char* Packet::constructPacket() {

	char* packet;
	int sizePacket = this->getSizePacket();

	//cout << "LOG. Taille du paquet = " << sizePacket << endl;

	if (sizePacket > MAX_SIZE_PAQUETS) {
		cout << "ERREUR. Le packet formet est plus grand que celui autorisé." << endl;
	}

	initBuffer(&packet, sizePacket);

	int iPacket = 0;

	for (int i=0; i < 3; i++) {
		packet[iPacket++] = this->char_id[i];
	}

	for (int i=0; i < 20; i++) {
		packet[iPacket++] = this->char_param[i];
	}

	char* constructSize;
	initBuffer(&constructSize, 9);
	sprintf(constructSize, "%d", sizeData);
	for (int i=0; i < 8; i++) {
		packet[iPacket++] = constructSize[i];
	}
	free(constructSize);

	if (this->haveData) {
		for (int i=0; i < this->sizeData; i++) {
			packet[iPacket++] = this->data[i];
		}
	}

	packet[iPacket++] = '\0';

	//cout << "data construct = " << this->data << endl;

	return packet;

}


Packet::Packet(char* packet) {
	
	int iPacket = 0;
	this->haveData = true;

	if (packet[0] == '\0') {
		cout << "Warning, packet vide" << endl;
	}

	initBuffer(&char_id, 4);
	for (int i=0; i < 3; i++) {
		char c = packet[iPacket++];
		if (c == '\0') {
			cout << "Warning, caractère d'ID introuvable." << endl;
		}
		this->char_id[i] = c;
	}

	initBuffer(&char_param, 21);
	for (int i=0; i < 20; i++) {
		this->char_param[i] = packet[iPacket++];
	}

	char* sizeDataChars;
	initBuffer(&sizeDataChars, 9);
	for (int i=0; i < 8; i++) {
		sizeDataChars[i] = packet[iPacket++];
	}
	this->sizeData = atoi(sizeDataChars);

	initBuffer(&data, this->sizeData);
	for (int i=0; i < this->sizeData; i++) {
		this->data[i] = packet[iPacket++];
	}

	//cout << "data out = " << this->data << ", size = " << this->sizeData << endl;

}


string Packet::getId() {
	return this->char_id;
}

string Packet::getArgument() {
	return this->char_param;
}

int Packet::getSizeData() {
	return this->sizeData;
}

char* Packet::getDatas() {
	return this->data;
}

int Packet::getSizePacket() {

	int sizePacket = 0;

	sizePacket += 3; // Taille de l'ID
	sizePacket += 20; // Taille du parametre
	sizePacket += 8; // Taille de l'entier

	if (this->haveData) {
		sizePacket += this->sizeData;
	}

	sizePacket += 1;

	return sizePacket;
}

void Packet::displayPacket(char* paquet, int size) {
	for (int i = 0; i < size; ++i)
	{
		char c = paquet[i];
		if (c == '\0')
			c = '0';

		if (c == '\n')
			c = 'n';

		cout << c;
	}

	cout << " | size=" << size << endl;
}

void Packet::display() {
	cout << "{" << endl;
	cout << "ID=" << this->getId() << endl;
	cout << "arg=" << this->getArgument() << endl;
	cout << "sizeData=" << this->getSizeData() << endl;
	cout << "data:";
	Packet::displayPacket(this->getDatas(), this->getSizeData());
	cout << "}" << endl;
}

void Packet::deleteFromMemory() {
	free(this->char_id);
	free(this->char_param);
	free(this->data);
}

