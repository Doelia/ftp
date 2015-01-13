#include "commun/header-commun.h"
#include "client/header-client.h"

void stop() {
    cout << "Fermerture..." << endl;
    View::getInstance()->close();
    exit(0);
}

int main(int args, char* argcs[]) {

    if (args < 2) {
        cout << "Veuillez spécifier l'adresse du serveur. Exemple : ./cli 127.0.0.1" << endl;
        return 0;
    }

    FileManager::init("./dir_client/");
    ThreadManager::init();
    Shell::init();
    View* view = View::init();
    FileReceiver::init(view);
    FileSender::init(view);
    NetworkManager* networkManager = NetworkManager::init();
 
	cout << "Lancement du client..." << endl;
    
    string ip = argcs[1];
    if (networkManager->connectToServer(ip, PORT_SERVER)) {
        networkManager->start_listenMessages();
        Shell::getInstance()->welcomeMsg();
        View::getInstance()->openView();
        Shell::getInstance()->promptCommand();
        sleep(1);
    }

    ThreadManager::getInstance()->joinAll();
    stop();
}
