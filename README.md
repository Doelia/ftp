ftp
===

Simple serveur FTP en C++

Permet d'échanger des fichiers entre un serveur et un client de façon simultannée.
Interface graphique en console.

Réalisé dans le cadre d'un cours universitaire de réseaux.

==

MODE D'EMPLOI

Compiler les executables avec 'make' a la racine du projet

# SERVEUR
- Lancer le serveur en se plaçant dans le dossier 'app_server', et executer ./serv
- Les fichiers se stockent dans un dossier dir_server, créé automatiquement s'il n'existe pas. Placez vos fichiers de tests à tranferer ici.
- Le port peut être changé dans le fichier commun/constantes.h

# CLIENT
- Lancer un client en se plaçant dans le dossier 'app_client', et executer ./cli
- L'application 'view' est la vue du programme, elle est executée automatiquement par l'application client au lancement.
- Nécessite l'application 'xterm' pour le lancement (présent sous linux)
- Les fichiers se stockent dans un dossier dir_client, créé automatiquement s'il n'existe pas. Placez vos fichiers de tests à tranferer ici.
- Commandes disponibles :
	- put <nom du fichier>
	- get <nom du fichier>

