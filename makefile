default:
	g++ -o serv serveur/*.cpp commun/*.cpp serveur.cpp -lpthread
	g++ -o cli commun/*.cpp client/*.cpp client.cpp -lpthread
serveur:
	g++ -o serv serveur/*.cpp commun/*.cpp serveur.cpp 
client:
	g++ -o cli commun/*.cpp client/*.cpp client.cpp -lpthread

