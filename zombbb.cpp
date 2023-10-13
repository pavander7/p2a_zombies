//9504853406CBAC39EE89AA3AD238AA12CA198043

#include <iostream>
#include <getopt.h>
#include <queue>
#include <deque>
#include "zombie.h"

using namespace std;

int main (int argc, char* argv[]) {
    //getoptlong time
    ios_base::sync_with_stdio(false);

    //cout << "good morning \n";

    static struct option long_options[] = {
    {"help",        no_argument,        NULL,  'h'},
    {"verbose",     no_argument,        NULL,  'v'},
    {"median",      no_argument,        NULL,  'm'},
    {"statistics",  required_argument,  0,     's'},
    {0,             0,                  NULL,  0, }
    };
    int option_index = 0;

    //variables for input processing
    bool verbose = false;
    bool median = false;
    bool statistics = false;
    uint32_t N = 0;

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
                statistics = true;
                N = uint32_t(stoi(optarg));
                //cout << "stats selected, N: " << N << endl;
                break;
        }
        c = getopt_long(argc, argv, "hvms:", long_options, &option_index);
    }

    //cout << "options parsed \n";
    //init simulation params
    uint32_t QC = 0;
    uint32_t seed = 0;
    uint32_t maxDist = 0;
    uint32_t maxSpeed = 0;
    uint32_t maxHealth = 0;
    
    //cin sim params
    string line;
    getline(cin, line); //discard comment
    cin >> line;
    cin >> QC; // quiver capacity
    cin >> line;
    cin >> seed; // seed
    cin >> line;
    cin >> maxDist; // max rand distance
    cin >> line;
    cin >> maxSpeed; // max rand speed
    cin >> line;
    cin >> maxHealth; // max rand health
    //cout << "sim params set \n";

    // init rand
    P2random::initialize(seed, maxDist, maxSpeed, maxHealth);

    //make player
    uint32_t round = 0;

    //for death tracking
    string killer; 
    string lastKill;
    bool dead = false;
    bool more = true;
    bool win = false;
    vector<Zombie*> graveyardF;
    deque<string> graveyardL;
    uint32_t numAlive = 0;

    //make zombie queue
    vector<Zombie> field;
    //priority_queue<Zombie, vector<Zombie>, ZombieCompare> field;

    //begin rounds
    getline(cin, line); // discard endl
    getline(cin, line); //discard delimiter
    uint32_t nextRound = 0;
    cin >> line >> nextRound; // round number
    while (!win || more) { 
        uint32_t quiver = 0;
        //cout << "step 1: start round \n";
        win = false;
        round++;
        if (verbose) cout << "Round: " << round << endl;
        
        //cout << "step 2: refill quiver \n";
        quiver = QC;

        //cout << "step 3: existing zombies move & attack \n";
        for (uint32_t r = 0; r < field.size(); r++) {
            if (field[r].alive) {
                bool reach = field[r].move();
                if (verbose) {
                    cout << "Moved: " << field[r] << "\n";
                }
                if (reach && !dead) {
                    killer = field[r].name;
                    dead = true;
                }
            }
        }

        //cout << "step 4: check death \n";
        if (dead) {
            cout << "DEFEAT IN ROUND " << round << "! " << killer << " ate your brains!\n";
            if (statistics) {
                cout << "Zombies still active: " << numAlive << endl;
                uint32_t sizer = uint32_t(graveyardF.size());
                cout << "First zombies killed:\n";
                for (uint32_t k = 0; k < min(N,sizer); k++) {
                    cout << graveyardF[k]->name << " " << k + 1 << endl;
                } 
                cout << "Last zombies killed:\n";
                for (uint32_t p = min(N,sizer); p > 0; p--) {
                    cout << graveyardL.front() << " " << p << endl;
                    graveyardL.pop_front();
                } 
                priority_queue<Zombie*, vector<Zombie*>, ZombLifeMost> most;
                for (uint32_t f = 0; f < field.size(); f++) {
                    most.push(&field[f]);
                }
                cout << "Most active zombies:\n";
                for (uint32_t g = 1; (g <= N && !most.empty()); g++) {
                    cout << most.top()->name << " " << most.top()->age << endl;
                    most.pop();
                }
                priority_queue<Zombie*, vector<Zombie*>, ZombLifeLeast> least;
                for (uint32_t f = 0; f < field.size(); f++) {
                    least.push(&field[f]);
                }
                cout << "Least active zombies:\n";
                for (uint32_t g = min(N,uint32_t(least.size())); g >= 1; g--) {
                    cout << least.top()->name << " " << least.top()->age << endl;
                    least.pop();
                }
            }
            return 0;
        }

        //cout << "step 5: new zombies appear\n";
        //cout << nextRound << " is the next wave\n";
        if (round == nextRound) {
            //cout << "hello\n";
            uint32_t numRandos = 0;
            uint32_t numOCs = 0;
            cin >> line >> numRandos; // random zombie pop
            cin >> line >> numOCs; // named zombie pop
            numAlive = numAlive + numRandos + numOCs;
            for (uint32_t q = 0; q < numRandos; q++) {
                Zombie temp = Zombie(false);
                field.push_back(temp);
                if (verbose) cout << "Created: " << temp << 
                //" eta: " << temp.eta() <<
                "\n";
            } for (uint32_t s = 0; s < numOCs; s++) {
                Zombie temp = Zombie(true);
                field.push_back(temp);
                if (verbose) cout << "Created: " << temp <<
                //" eta: " << temp.eta() <<
                "\n";
            }
            getline(cin, line); //discard endl
            getline(cin, line); //discard delimiter
            if (!(cin >> line >> nextRound)) { // round number
                more = false;
                //cout << "no more waves \n";
            }
        }

        //cout << "step 6: player shoots zombies\n";
        priority_queue<Zombie*, vector<Zombie*>, ZombieCompare> shooting_range;
        for (uint32_t k = 0; k < field.size(); k++) {
            if (field[k].alive) shooting_range.push(&field[k]);
        }
        while (quiver != 0 && !shooting_range.empty()) {
            //cout << "quiver: " << quiver << endl;
            Zombie* temp = shooting_range.top();
            shooting_range.pop();
            temp->damage();
            quiver--;
            //cout << "shooting: " << *temp << endl;
            if (temp->die()) {
                if (shooting_range.empty()) win = true;
                numAlive--;
                if (verbose) {
                    cout << "Destroyed: " << *temp << endl;
                }
                lastKill = temp->name;
                if (statistics) {
                    if (graveyardL.size() >= N) {
                        graveyardL.pop_back();
                        graveyardL.push_front(temp->name);
                    } else {
                        graveyardF.push_back(temp);
                        graveyardL.push_front(temp->name);
                    }
                }
            } else {
                shooting_range.push(temp);
                //cout << *temp << " survived \n";
            } //cout << shooting_range.size() << " zombies remaining \n";
            //cout << "condition is " << (quiver != 0 && !shooting_range.empty()) << endl;
        } 

        //cout << "sim exited\n";

        //cout << "step 7: median\n";
        
        if (median) {
            //cout << "hi from median\n";
            size_t iMedian = 0;
            uint32_t medVal = 0;
            priority_queue<Zombie*, vector<Zombie*>, ZombLifeLeast> ages;
            for (uint32_t p = 0; p < field.size(); p++) {
                if (!field[p].alive) {
                    ages.push(&field[p]);
                    //cout << "Median: " << field[p] << endl;
                }
            }  //cout << field.size() << endl;
            if (ages.size()%2 == 0) {
                iMedian = ages.size()/size_t(2) - size_t(1);
                //cout << iMedian << endl;
                if (iMedian != 0) {
                    for (size_t w = 0; w < iMedian-1; w++) {
                        ages.pop();
                    } 
                } Zombie* temp = ages.top();
                ages.pop();
                medVal = (ages.top()->age + temp->age)/uint32_t(2);
            } else {
                iMedian = (ages.size() - size_t(1))/size_t(2);
                //cout << iMedian << endl;
                for (size_t w = 0; w < iMedian-1; w++) {
                    ages.pop();
                } medVal = ages.top()->age;
            }
            cout << "At the end of round " << round << ", the median zombie lifetime is " << medVal << endl;
        } 
        
        // step 8: check win (while loop condition)
    }
    cout << "VICTORY IN ROUND " << round << "! " << lastKill << " was the last zombie.\n";
    // stats output
    if (statistics) {
        cout << "Zombies still active: 0\n";
        uint32_t sizer = uint32_t(graveyardF.size());
        cout << "First zombies killed:\n";
        for (uint32_t k = 0; k < min(N,sizer); k++) {
            cout << graveyardF[k]->name << " " << k + 1 << endl;
        } 
        cout << "Last zombies killed:\n";
        for (uint32_t p = min(N,sizer); p > 0; p--) {
            cout << graveyardL.front() << " " << p << endl;
            graveyardL.pop_front();
        } 
        priority_queue<Zombie*, vector<Zombie*>, ZombLifeMost> most;
        for (uint32_t f = 0; f < field.size(); f++) {
            most.push(&field[f]);
        }
        cout << "Most active zombies:\n";
        for (uint32_t g = 1; (g <= N && !most.empty()); g++) {
            cout << most.top()->name << " " << most.top()->age << endl;
            most.pop();
        }
        priority_queue<Zombie*, vector<Zombie*>, ZombLifeLeast> least;
        for (uint32_t f = 0; f < field.size(); f++) {
            least.push(&field[f]);
        }
        cout << "Least active zombies:\n";
        for (uint32_t g = min(N,uint32_t(least.size())); g >= 1; g--) {
            cout << least.top()->name << " " << least.top()->age << endl;
            least.pop();
        }
    }
    return 0;
}