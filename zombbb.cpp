//9504853406CBAC39EE89AA3AD238AA12CA198043

#include <iostream>
#include <getopt.h>
#include <queue>
#include "zombie.h"

using namespace std;

int main (int argc, char* argv[]) {
    //getoptlong time
    ios_base::sync_with_stdio(false);

    static struct option long_options[] = {
    {"help",        no_argument,        NULL,  'h'},
    {"verbose",     no_argument,        NULL,  'v'},
    {"median",      no_argument,        NULL,  'm'},
    {"statistics",  required_argument,  0,     's'},
    {0,             0,                  NULL,  0, }
    };
    int option_index = 0;

    //placeholder variables for input processing
    bool verbose = false;
    bool median = false;
    uint32_t statistics = 0;

    int c = getopt_long(argc, argv, "hvms:", long_options, &option_index);
    while (c != -1) {
        switch(c) {
            case 'h' :
                cout << "help message";
                return 0;
                break;
            case 'v' :
                verbose = true;
                break;
            case 'm' :
                median = true;
                break;
            case 's' :
                statistics = uint32_t(optarg);
                break;
        }
    }

    //init simulation params
    uint32_t QC = 0;
    uint32_t seed = 0;
    uint32_t maxDist = 0;
    uint32_t maxSpeed = 0;
    uint32_t maxHealth = 0;

    //cin sim params
    string line;
    getline(cin, line); //discard comment
    cin >> line >> QC; // quiver capacity
    cin >> line >> seed; // seed
    cin >> line >> maxDist; // max rand distance
    cin >> line >> maxSpeed; // max rand speed
    cin >> line >> maxHealth; // max rand health

    // init rand
    P2random::initialize(seed, maxDist, maxSpeed, maxHealth);

    //make player
    uint32_t quiver = 0;
    uint32_t round = 1;
    uint32_t numRandos = 0;
    uint32_t numOCs = 0;

    //for death tracking
    string killer; 
    string lastKill;
    bool dead = false;
    bool more = true;

    //make zombie queues
    priority_queue<Zombie> field;

    //begin rounds
    getline(cin, line); //discard delimiter
    uint32_t nextRound = 0;
    cin >> line >> nextRound; // round number
    while (!field.empty() || more) { 
        // step 1: start round
        if (verbose) cout << "Round: " << round << endl;
        
        // step 2: refill quiver
        quiver = QC;

        // step 3: existing zombies move & attack
        priority_queue<Zombie> old_field;
        for (uint32_t w = 0; w < field.size(); w++) {
            Zombie temp = field.top();
            field.pop();
            bool reach = temp.move();
            if (verbose) {
                cout << "Moved: " << temp << "\n";
            }
            if (reach && !dead) {
                killer = temp.name;
                dead = true;
            }
            old_field.push(temp);
        }
        field = old_field;

        // step 4: check death
        if (dead) {
            cout << "DEFEAT IN ROUND " << round << "! " << killer << " ate your brains!\n";
            return 0;
        }

        // step 5: new zombies appear
        if (round = nextRound) {
            cin >> line >> numRandos; // random zombie pop
            cin >> line >> numOCs; // named zombie pop
            for (uint32_t q = 0; q < numRandos; q++) {
                Zombie temp = Zombie(false);
                field.push(temp);
                if (verbose) cout << "Created: " << temp << "\n";
            } for (uint32_t s = 0; s < numOCs; s++) {
                Zombie temp = Zombie(true);
                field.push(temp);
                if (verbose) cout << "Created: " << temp << "\n";
            }
            getline(cin, line); //discard delimiter
            if (!(cin >> line >> nextRound)) { // round number
                more = false;
            }
        }

        // step 6: player shoots zombies
        while (quiver != 0) {
            Zombie temp = field.top();
            field.pop();
            temp.damage();
            if (temp.die()) {
                if (verbose) {
                    cout << "Destroyed: " << temp << endl;
                }
                lastKill = temp.name;
            } else {
                field.push(temp);
            }
        }

        // step 7: median

        // step 8: check win (while loop condition)
    }
    cout << "VICTORY IN ROUND " << round << "! " << lastKill << " was the last zombie.\n";
}
