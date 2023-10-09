//9504853406CBAC39EE89AA3AD238AA12CA198043

#include <string>
#include <iostream>
#include "P2random.h"

using namespace std;

class Zombie {
public:
    Zombie(string nameIn, uint32_t distanceIn, uint32_t speedIn, uint32_t healthIn);
    Zombie(bool named);
    
    void damage();
    bool move();

    uint32_t eta() const;

    bool die();

    friend class ZombieCompare;
    friend ostream & operator << (ostream &out, const Zombie &z);

    string name;
    uint32_t age;

private:
    uint32_t distance;
    uint32_t speed;
    uint32_t health;
    bool alive;
};

class ZombieCompare {
public:
    bool operator() (const Zombie &a, const Zombie &b) const;
};