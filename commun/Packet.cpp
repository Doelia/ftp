#include "Packet.h"

Packet::Packet() {

	initBuffer(&char_id, 4);
	initBuffer(&char_param, 20);
}

Packet::Packet(string p_id, string p_param, int p_sizeData, char* p_data) {

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
		char s[] = "";
		this->data = s;
	} else {
		this->data = p_data;
	}

}

char* Packet::constructPacket() {

	char* packet;
	initBuffer(&packet, 3+20+8+this->sizeData);

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

	for (int i=0; i < this->sizeData; i++) {
		packet[iPacket++] = this->data[i];
	}

	//cout << "data construct = " << this->data << endl;

	return packet;

}


Packet::Packet(char* packet) {
	
	int iPacket = 0;

	initBuffer(&char_id, 4);
	for (int i=0; i < 3; i++) {
		this->char_id[i] = packet[iPacket++];
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

	initBuffer(&data, this->sizeData+1);
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

