#ifndef PACKET_H
#define PACKET_H

#include "header-commun.h"

using namespace std;

class Packet {
	public:
		string getId();
		string getArgument();
		Packet();
		Packet(string p_id, string p_param, int p_sizeData, char* p_data);
		Packet(char* packet);
		char* constructPacket();
		int getSizeData();
		char* getDatas();

		
	private:
		char* char_id; // 3 caractères pour décrire le paquet
		char* char_param; // taille de 20
		int sizeData;
		char* data;
};

#endif