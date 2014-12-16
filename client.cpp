#include "commun/header-commun.h"
#include "client/header-client.h"

int main() {

    ThreadManager::init();
    Shell::init();
    FileReceiver::init(View::init());
    NetworkManager* networkManager = NetworkManager::init();
 
	cout << "Lancement du client..." << endl;
    
    if (networkManager->connectToServer("127.0.0.1", PORT_SERVER)) {
        networkManager->start_listenMessages();

        Shell::getInstance()->welcomeMsg();
        View::getInstance()->openView();
        Shell::getInstance()->promptCommand();
        sleep(1);
    }

    ThreadManager::getInstance()->joinAll();
}

