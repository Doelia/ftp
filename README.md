Programme type FTP
===

Permet d'échanger des fichiers entre un serveur et un client de façon simultannée.
Interface graphique en console.

==

## Compilation
- Compiler les executables avec 'make' a la racine du projet

## Serveur
- Lancer le serveur en se plaçant dans le dossier 'app_server', et executer ./serv
- Les fichiers se stockent dans un dossier dir_server, créé automatiquement s'il n'existe pas. Placez vos fichiers de tests à tranferer ici.
- Le port peut être changé dans le fichier commun/constantes.h

## Client
- Lancer un client en se plaçant dans le dossier 'app_client', et executer ./cli
- L'application 'view' est la vue du programme, elle est executée automatiquement par l'application client au lancement.
- Nécessite l'application 'xterm' pour le lancement (présent sous linux)
- Les fichiers se stockent dans un dossier dir_client, créé automatiquement s'il n'existe pas. Placez vos fichiers de tests à tranferer ici.
- Commandes disponibles :
	- put <nom du fichier>
	- get <nom du fichier>

## TODO
- Gérer le PUT
- Rétablir la view
- Vérifier la fermerture propre
- Passer l'IP par parametre au client