#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

void recevoirFichier(char path[], int sock) {

	char sortie[250] = {0};
	sprintf(sortie, "downloads/%s", path);
	int fd = open(sortie, O_CREAT | O_WRONLY, 777);
	int retourWrite;
	
	if(fd != -1) {

		char buffer[256] = {0};
		recv(sock, buffer, 256, 0);
		int nbrChar = atoi(buffer);

		cout << "Taille du fichier: " << nbrChar << endl;

		int cpt = 0;
		cout << "En attente de paquets...\n";
		while((recv(sock, buffer, 1, 0)) > 0) {

			cpt++;
			char c = buffer[0];

			if(!write(fd, buffer, 1)) {
				cout << "Erreur client : Erreur lors de l'écriture du fichier.";
				return;
			}

			if (cpt >= nbrChar) {
				cout << "Fin de fichier detectée." << endl;
				close(fd);
				return;
			}
		}
	}
	else {
		cout << "Erreur client : Impossible de créer le fichier. Vérifiez s'il n'existe pas déjà dans les downloads." << endl;
		exit(0);
	}
}

int main(int argc, char **argv) {

	if (argc != 3) {
		cout << "Veuillez spécifiez IP et PORT en paramètres.\n";
		return 0;
	}

	int port = atoi(argv[2]);
	string ip = argv[1];

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock != -1) {

		struct sockaddr_in addr;
		addr.sin_addr.s_addr    = inet_addr(ip.c_str());
		addr.sin_family         = AF_INET;
	 	addr.sin_port           = htons(port);

	 	cout << "Connexion à " << ip << ":" << port << "..." << endl;
		if (connect(sock, (sockaddr*)&addr, sizeof(addr)) != -1) {
			cout << "Connecté au seveur !\n";

			while (true) {
				char buffer[100] = {0};

				// Récupération message utilisateur
				cout << "========================================" << endl;
				cout << "Commandes disponibles : get <file>, stop" << endl;
				cout << "Exemple : get test.txt, get tp.pdf" << endl;
				cout << "> ";
				cin.getline(buffer, 100);

				if (!strlen(buffer)) {
					continue;
				}

				// Si c'est stop, on coupe le programme
				if (!strcmp(buffer, "stop")) {
					close(sock);
					return 0;
				}

				// Envoi message tapé au serveur
				int sock_err = send(sock, buffer, 100, 0);
	            if(sock_err != -1)
	            {
	            	// On écoute la réponse
	            	char buf[1024] = {0};
	            	cout << "En attente du serveur..." << endl;
					recv(sock, buf, 1024, 0);
					cout << "reçu: " << buf << "\n";

					// On test si on reçoit un message de téléchargement de fichier
					if(!strcmp(buf, "DL_GO")) {
						cout << "Reception du fichier en cours..." << endl;
						recevoirFichier(buffer, sock);
						cout << "Fin de récéption. Fichié sauvegardé dans " << "downloads/" << buffer << endl;
					}
					
	            }
	            else
	               cout << "Erreur de transmission à l'envoi du message\n";
			}

		}
		else {
			cout << "Impossible de se connecter au serveur.\n";
		}
	}
	else {
		cout << "Erreur creation de socket\n";
	}
}

