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

void Zombie::move() {
    distance -= min(distance, speed);
}

void Zombie::damage() {
    health--;
}

uint32_t Zombie::eta() {
    return distance/speed;
}

bool Zombie::die() {
    if (health == 0) {
        alive = false;
    } return alive;
}

Zombie Zombie::operator<(Zombie &right) {
    if (this->eta() < right.eta()) {
        return *this;
    } else if (this->eta() == right.eta()) {
        if (this->health < right.health) {
            return *this;
        } else if ()
    }
}