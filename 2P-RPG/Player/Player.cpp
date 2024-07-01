//
// Created by Victor Navarro on 15/02/24.
//

#include "Player.h"
#include <iostream>
#include "../Files/FileHandler.h"
#include <random>
#include <cstring>


using namespace std;

void Player::saveProgress() {
    char* buffer = this->serialize();
    FileHandler fileHandler = FileHandler();

    fileHandler.writeToFile("PlayerInfo.data", buffer, BUFFER_SIZE);
    cout << "Saved..." << endl;

    delete[] buffer;
}
Player::Player(char* _name, int _health, int _attack, int _defense, int _speed) :
Character(_name, _health, _attack, _defense, _speed, true),
maxHealth(_health) {
    level = 1;
    experience = 0;
}

Player::Player(char* _name, int _health, int _attack, int _defense, int _speed, bool _isPlayer, int _level, int _experience) :
Character(_name, _health, _attack, _defense, _speed, _isPlayer) {
    level = _level;
    experience = _experience;
}

void Player::doAttack(Character *target) {
    int initialHealth = health;

    target->takeDamage(attack);

    if (target->getHealth() <= 0) { // Verificamos si le dimos en la madre al enemigo
        cout << name << " defeated " << target->getName() << "!" << endl;

        // RECOMPENSA POR MADREAR ENEMIGO
        int randomReward = rand() % 2; // Genera un número aleatorio entre 0 y 1

        switch (randomReward) {
            case 0: {
                // la mitad de su vida máxima
                int healAmount = maxHealth / 2;
                health = min(maxHealth, health + healAmount);
                cout << "You found a potion and healed " << healAmount << " HP!" << endl;
                break;
            }
            case 1: {
                // 50% extra de la experiencia
                int extraExperience = ((Enemy *)target)->getExperience() / 2;
                gainExperience(((Enemy *)target)->getExperience() + extraExperience);
                cout << "You gained " << extraExperience << " extra experience!" << endl;
                break;
            }
            default:
                break;
        }

        // Si sale 1 muestra la vida agregada
        if (initialHealth != health) {
            cout << "Current health: " << health << "/" << maxHealth << endl;
        }
    }
}

void Player::takeDamage(int damage) {
    damage = damage * level;  // Aumento de ataque del enemigo en cada nivel
    int trueDamage = damage - defense;

    health-= trueDamage;
    cout << name << " took " << trueDamage << " damage!" << endl;

    if(health <= 0) {
        cout << name << " has been defeated!" << endl;
    }
}

//Aumento de nivel para los enemigos cuando sube el personaje
void Player::levelUpEnemies(const std::vector<Enemy*>& enemies) {
    for (auto enemy : enemies) {
        enemy->increaseStats(5); // Aumenta las estadísticas de los enemigos en 5 puntos
    }
}
void Player::levelUp() {
    int points = 10;
    level++;
    health += points;
    attack += points;
    defense += points;
    speed += points;

    cout << "::::::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << "Your increase level: " << level << endl;
    cout << "Health: " << health << ", Attack: " << attack << ", Defense: " << defense << ", Speed: " << speed << endl;

    levelUpEnemies(enemies);

}

void Player::gainExperience(int exp) {
    experience += exp;
    cout << "You win: " << exp << " experience" <<endl;
    if (experience >= 100) {
        experience -= 100;
        levelUp();
    cout << "You have " << experience << "experience" << endl;
    }
}

Character* Player::selectTarget(vector<Enemy*> possibleTargets) {
    int selectedTarget = 0;
    bool invalid = true;
    do {
        cout << "Select a target: " << endl;
        for (int i = 0; i < possibleTargets.size(); i++) {
            cout << i << ". " << possibleTargets[i]->getName() << endl;
        }
        cin >> selectedTarget;
        for (int i = 0; i < possibleTargets.size(); i++) {
            if (selectedTarget == i) {
                invalid = false;
                break;
            }
        }
        if (invalid) {
            cout << "Invalid option" << endl;
        }
    } while (invalid);
    return possibleTargets[selectedTarget];
}


Action Player::takeAction(vector<Enemy*> enemies) {
    int action;
    setEnemies(enemies);
    cout << "Select an action: " << endl
         << "1. Attack" << endl
         << "2. Defend your soul" << endl
         << "3. Save Progress " << endl;
    cin >> action;
    Action currentAction;
    Character* target = nullptr;
    switch(action)
    {
        case 1:
            target = selectTarget(enemies);
            currentAction.target = target;
            currentAction.action = [this, target](){
                doAttack(target);
                if (target -> getHealth() <= 0)
                {
                    this->gainExperience(((Enemy *) target)->getExperience());

                }
            };
            currentAction.speed = getSpeed();
            break;
        case 2:
            currentAction.target = target;
            currentAction.action = [this]()
            {
                defend();
            };
            currentAction.speed = 999999;
            break;
        case 3:
            saveProgress();
            return takeAction(enemies);
            break;
        default:
            cout << "Invalid action" << endl;
            currentAction.action = nullptr;
            break;
    }

    return currentAction;
}


char* Player::serialize() {
    char* iterator = buffer;

    memcpy(iterator, &name, sizeof(name));
    iterator += sizeof(name);

    memcpy(iterator, &health, sizeof(health));
    iterator += sizeof(health);

    memcpy(iterator, &attack, sizeof(attack));
    iterator += sizeof(attack);

    memcpy(iterator, &defense, sizeof(defense));
    iterator += sizeof(defense);

    memcpy(iterator, &speed, sizeof(speed));
    iterator += sizeof(speed);

    memcpy(iterator, &isPlayer, sizeof(isPlayer));
    iterator += sizeof(isPlayer);

    memcpy(iterator, &level, sizeof(level));
    iterator += sizeof(level);

    memcpy(iterator, &experience, sizeof(experience));

    return buffer;
}

Player* Player::unserialize(char* buffer) {
    char* iterator = buffer;
    char name[30];
    int health, attack, defense, speed, level, experience;
    bool isPlayer;

    memcpy(&name, iterator, sizeof(name));
    iterator += sizeof(name);

    memcpy(&health, iterator, sizeof(health));
    iterator += sizeof(health);

    memcpy(&attack, iterator, sizeof(attack));
    iterator += sizeof(attack);

    memcpy(&defense, iterator, sizeof(defense));
    iterator += sizeof(defense);

    memcpy(&speed, iterator, sizeof(speed));
    iterator += sizeof(speed);

    memcpy(&isPlayer, iterator, sizeof(isPlayer));
    iterator += sizeof(isPlayer);

    memcpy(&level, iterator, sizeof(level));
    iterator += sizeof(level);

    memcpy(&experience, iterator, sizeof(experience));
    iterator += sizeof(experience);

    return new Player(name, health, attack, defense, speed, isPlayer, level, experience);
}

void Player::setEnemies(const std::vector<Enemy*>& _enemies) {
    enemies = _enemies;
}

