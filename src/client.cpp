#include "commun/header-commun.h"
#include "client/header-client.h"

void stop() {
    cout << "Fermerture..." << endl;
    View::getInstance()->close();
    exit(0);
}

int main() {

    FileManager::init("./dir_client/");
    ThreadManager::init();
    Shell::init();
    View* view = View::init();
    FileReceiver::init(view);
    FileSender::init(view);
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
    stop();
}