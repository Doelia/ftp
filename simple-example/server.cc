#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 10928

using namespace std;

void envoyerFichier(char nomFichier[], int sockClient) {

	int descriptFichier;

	if ((descriptFichier = open(nomFichier, O_RDONLY)) != -1) {
		cout << "Envoi de " << nomFichier << " a " << sockClient << endl;

		// Envoie du feu vert
		send(sockClient, "DL_GO", 20, 0);

		// Buffer de lecture propre
		char bufferFile[5] = {0};
		for(int i=0;i<5;i++) {
			bufferFile[i] = '\0';
		}

		// On compte le nombre de caractères avant d'envoyer
		int nbrChar = 0;
		while(read(descriptFichier, bufferFile, 1)) {
			nbrChar++;
		}

		// On envoie la taille du fichier
		char buffer[256] = {0};
		sprintf(buffer, "%d", nbrChar);
		send(sockClient, buffer, 256, 0);
		cout << "Taille de fichier envoyée : " << buffer << endl;

		// On se remet au début du fichier
		close(descriptFichier);
		descriptFichier = open(nomFichier, O_RDONLY);

		cout << "Envoi du fichier en cours..." << endl;
		// On envoie tout le contenu du fichier
		while(read(descriptFichier, bufferFile, 1)) {

			if (!send(sockClient, bufferFile, 1, 0)) {
				cout << "Erreur de transmission\n";
				close(descriptFichier);
				return;
			}

			bufferFile[0] = '\0';
		}

		cout << "Fin d'envoi fu cihier" << endl;

	}
	else {
		char message[200] = "Erreur : Fichié demandé introuvable";
		int sock_err = send(sockClient, message, 200, 0);
		if (sock_err != -1)
			cout << "Message envoyé au client : " << message << "\n";
		else
			cout << "[log server] Erreur de transmission\n";
	}

	close(descriptFichier);

}

void gererClient(int sockClient) {
	cout << "Attente d'un messagea...\n";
	char buf[32] = {0};

	while (int retour = recv(sockClient, buf, 32, 0)) {
		if (strlen(buf)) {
			cout << "message reçu du client = " << buf << "\n";

			if (!strcmp(buf, "stop")) {
				close(sockClient);
				return;
			}
			else {

				char* pch = strtok(buf, " ");

				// Detection "get"
				if (!strcmp(pch, "get")) {

					char* path = strtok(NULL, " ");
					cout << "[log server] Ficher demandé: " << path << endl;
					envoyerFichier(path, sockClient);
					cout << "[log server] fichié envoyé avec succès." << endl;

				}
				else { // Commande introuvable
					char message[200] = "Erreur : Commande introuvable";
					int sock_err = send(sockClient, message, 200, 0);
					if (sock_err != -1)
						cout << "Message envoyé au client : " << message << "\n";
					else
						cout << "Erreur de transmission\n";
				}
			}
		}
	}
	
}

int main() {

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock != -1) {

		struct sockaddr_in addr;
		addr.sin_addr.s_addr    = htonl(INADDR_ANY);
		addr.sin_family         = AF_INET;
	 	addr.sin_port           = htons(PORT);
	 	
		int retourBind = bind(sock, (sockaddr*)&addr, sizeof(addr));
		
		if (retourBind != -1) {
			cout << "Bind OK\n";

			int retourListen = listen(sock, 5);
			if (retourListen != -1) {
				cout << "listen() sur le port " << PORT << " OK\n";

				sockaddr_in addrClient;
				socklen_t size = sizeof(addrClient);
				int sockClient;

				cout << "En attente de connexion...\n";
				while (sockClient = accept(sock, (sockaddr*)&addrClient, &size)) {

					cout << "Un client se connecte avec la socket "<< sockClient << " de " << inet_ntoa(addrClient.sin_addr) << ":"<<htons(addrClient.sin_port)<<"\n";

					int pid = fork();

					if (pid) { // Pere
						wait(pid); // On attend le fils
					}
					else { // Fils
						gererClient(sockClient);
					}
					cout << "En attente de connexion...\n";
				}
			}
			else {
				cout << "Impossible d'écouter sur le port " << PORT << "\n";
			}
		}
		else {
			cout << "Bind de la socket sur le port " << PORT << " impossible\n";
		}

	}
	else {
		cout << "Erreur creation de socket\n";
	}
}



