default:
	g++ -o app_server/serv src/serveur/*.cpp src/commun/*.cpp src/serveur.cpp -lpthread
	g++ -o app_client/cli src/commun/*.cpp src/client/*.cpp src/client.cpp -lpthread
	gcc -o app_client/viewer src/client-view.c
	