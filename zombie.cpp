//9504853406CBAC39EE89AA3AD238AA12CA198043

#include "zombie.h"

using namespace std;

Zombie::Zombie(string nameIn, uint32_t distanceIn, uint32_t speedIn, uint32_t healthIn) {
    name = nameIn;
    distance = distanceIn;
    speed = speedIn;
    health = healthIn;
    age = 0;
    alive = true;
}

Zombie::Zombie(bool named) {
    string junk;
    if (named) {
        cin >> name >> junk >> distance >> junk >> speed >> junk >> health;
    } else {
        string       nameIn = P2random::getNextZombieName()     ;
        uint32_t distanceIn = P2random::getNextZombieDistance() ;
        uint32_t    speedIn = P2random::getNextZombieSpeed()    ;
        uint32_t   healthIn = P2random::getNextZombieHealth()   ;
        name                = nameIn;
        distance            = distanceIn;
        speed               = speedIn;
        health              = healthIn;
    }
    age   = 0;
    alive = true;
}

bool Zombie::move() {
    distance -= min(distance, speed);
    return distance == 0;
}

void Zombie::damage() {
    health--;
}

uint32_t Zombie::eta() const {
    return distance/speed;
}

bool Zombie::die() {
    if (health == 0) {
        alive = false;
    } return !alive;
}

bool ZombieCompare::operator() (const Zombie &a, const Zombie &b) const {
    if (a.eta() < b.eta()) {
        return true;
    } else if (a.eta() == b.eta()) {
        if (a.health < b.health) {
            return true;
        } else if (a.health == b.health) {
            return (a.name < b.name);
        } else return false;
    } else return false;
}

ostream & operator << (ostream &out, const Zombie &z) {
    out << z.name << " (distance: " << z.distance << ", speed: " << z.speed
        << ", health: " << z.health << ")";
    return out;
}