default:
	g++ -o serv serveur/*.cpp commun/*.cpp serveur.cpp -lpthread
	g++ -o cli commun/*.cpp client/*.cpp client.cpp -lpthread
	gcc -o viewer client-view.c
	