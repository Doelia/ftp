#ifndef PACKET_H
#define PACKET_H

class Packet;

#include "header-commun.h"

using namespace std;

class Packet {
	public:
		string getId();
		string getArgument();
		Packet(string p_id, string p_param, int p_sizeData, char* p_data);
		Packet(char* packet, int size);
		char* constructPacket();
		int getSizeData();
		int getSizePacket();
		char* getDatas();
		void display();
		void deleteFromMemory();

		static void displayPacket(char*, int);
		static int getSizeHeaders();

		
	private:
		char* char_id; // 3 caractères pour décrire le paquet
		char* char_param; // taille de 20
		int sizeData;
		char* data;
		bool haveData;

		bool isIdPacketWithoutData();
};

#endif