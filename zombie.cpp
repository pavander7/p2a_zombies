//9504853406CBAC39EE89AA3AD238AA12CA198043

#include "zombie.h"

using namespace std;

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
    age   = 1;
    alive = true;
}

bool Zombie::move() {
    distance -= min(distance, speed);
    age++;
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

bool ZombieCompare::operator() (const Zombie* a, const Zombie* b) const {
    if (a->eta() < b->eta()) {
        return false;
    } else if (a->eta() == b->eta()) {
        if (a->health < b->health) {
            return false;
        } else if (a->health == b->health) {
            return (a->name > b->name);
        } else return true;
    } else return true;
}

bool ZombLifeLeast::operator() (const Zombie* a, const Zombie* b) const {
    if (a->age == b->age) {
        return (a->name > b->name);
    } else return (a->age > b->age);
}

bool ZombLifeMost::operator() (const Zombie* a, const Zombie* b) const {
    if (a->age == b->age) {
        return (a->name > b->name);
    } else return (a->age < b->age);
}

ostream & operator << (ostream &out, const Zombie &z) {
    out << z.name << " (distance: " << z.distance << ", speed: " << z.speed
        << ", health: " << z.health << ")";
    return out;
}