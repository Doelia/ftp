#include "commun/header-commun.h"
#include "client/header-client.h"

void stop() {
    cout << "Fermerture..." << endl;
    //View::getInstance()->close();
    exit(0);
}

int main() {

    ThreadManager::init();
    Shell::init();
    FileReceiver::init(View::init());
    NetworkManager* networkManager = NetworkManager::init();
 
	cout << "Lancement du client..." << endl;
    
    if (networkManager->connectToServer("10.5.12.12", PORT_SERVER)) {
        networkManager->start_listenMessages();

        Shell::getInstance()->welcomeMsg();
        View::getInstance()->openView();
        Shell::getInstance()->promptCommand();
        sleep(1);
    }

    ThreadManager::getInstance()->joinAll();
    stop();
}

