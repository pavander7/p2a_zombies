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

    //make player
    uint32_t quiver = 0;
    uint32_t round = 0;
    uint32_t numRandos = 0;
    uint32_t numOCs = 0;

    //make zombie queues
    priority_queue<Zombie> field;

    //begin rounds
    getline(cin, line); //discard delimiter
    while (true) {
        refill(QC);
        cin >> line >> round; // round number
        cin >> line >> numRandos; // random zombie pop
        cin >> line >> numOCs; // named zombie pop
        for (uint32_t q = 0; q < numOCs; q++) {
            
        }
    }
}

void refill(uint32_t QC) {

}