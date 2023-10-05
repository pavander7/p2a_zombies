//9504853406CBAC39EE89AA3AD238AA12CA198043

#include <string>

using namespace std;

class Zombie {
public:
    Zombie(string nameIn, uint32_t distanceIn, uint32_t speedIn, uint32_t healthIn);
    
    void damage();
    void move();

    uint32_t eta();

    bool die();

    Zombie operator<(Zombie &right);

private:
    string name;
    uint32_t distance;
    uint32_t speed;
    uint32_t health;
    uint32_t age;
    bool alive;
};