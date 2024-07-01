#include <iostream>
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Combat/Combat.h"
#include "Files/FileHandler.h"

Player* loadPlayerInfo() {
    try {
        char buffer[Player::BUFFER_SIZE];
        FileHandler fileHandler = FileHandler();
        fileHandler.readFromFile("PlayerInfo.data", buffer, Player::BUFFER_SIZE);
        return Player::unserialize(buffer);
    } catch(int error) {
        //Preguntar al usuario el nombre de jugador
        return new Player("DanieliniCaporal", 20, 20, 20, 8);
    }
}

int main() {
    int aux;
    do{
    Player *player = loadPlayerInfo();
    cout << player->getName() << endl;
    cout << player->toString() << endl;
    Enemy *enemy = new Enemy("Goblin", 15, 6, 2, 5, 75);
    Enemy *enemy2 = new Enemy("Orc", 15, 6, 2, 5, 75);

    vector<Character*> participants;

    participants.push_back(player);
    participants.push_back(enemy);
    participants.push_back(enemy2);

    Combat *combat = new Combat(participants);
    combat->doCombat();

    delete player;
    delete enemy;
    delete combat;

    cout << "Start again? " << endl << " 1. Si\n 2. No " << endl;
    cin >> aux;
    } while (aux == 1);
    return 0;
}
