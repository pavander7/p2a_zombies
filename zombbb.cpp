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
    uint32_t quiver = 0;
    uint32_t round = 0;
    uint32_t numRandos = 0;
    uint32_t numOCs = 0;

    //for death tracking
    string killer; 
    string lastKill;
    bool dead = false;
    bool more = true;
    bool win = false;
    uint32_t NUM_ZOMBIES = 0;
    deque<string> graveyardF;
    deque<string> graveyardL;
    uint32_t maxAge = 1;
    bool firstkill = true;
    uint32_t minAge = 1;

    //make zombie queue
    vector<Zombie> field;
    //priority_queue<Zombie, vector<Zombie>, ZombieCompare> field;

    //begin rounds
    getline(cin, line); // discard endl
    getline(cin, line); //discard delimiter
    uint32_t nextRound = 0;
    cin >> line >> nextRound; // round number
    while (!win || more) { 
        //cout << "step 1: start round \n";
        round++;
        if (verbose) cout << "Round: " << round << endl;
        
        //cout << "step 2: refill quiver \n";
        quiver = QC;

        //cout << "step 3: existing zombies move & attack \n";
        for (uint32_t urethra = 0; urethra < field.size(); urethra++) {
            if (field[urethra].alive) {
                bool reach = field[urethra].move();
                if (verbose) {
                    cout << "Moved: " << field[urethra] << "\n";
                }
                if (reach && !dead) {
                    killer = field[urethra].name;
                    dead = true;
                }
            }
        }

        //cout << "step 4: check death \n";
        if (dead) {
            cout << "DEFEAT IN ROUND " << round << "! " << killer << " ate your brains!\n";
            if (statistics) {
                uint32_t numAlive = 0;
                for (uint32_t uvula = 0; uvula < field.size(); uvula++) {
                    Zombie temp = field[uvula];
                    if (temp.alive) {
                        numAlive++;
                        if (firstkill) {
                            //cout << "FATALITY\n";
                            maxAge = minAge = temp.age;
                            firstkill = false;
                        } else {
                            if (temp.age > maxAge) {
                                maxAge = temp.age;
                            } if (temp.age < minAge) {
                                minAge = temp.age;
                            }
                        }
                    } 
                }
                cout << "Zombies still active: " << numAlive << endl;
                cout << "First zombies killed:\n";
                uint32_t sizer = uint32_t(graveyardF.size());
                for (uint32_t k = 1; k <= min(N,sizer); k++) {
                    cout << graveyardF.front() << " " << k << endl;
                    graveyardF.pop_front();
                } 
                cout << "Last zombies killed:\n";
                for (uint32_t p = 0; p < min(N,sizer); p++) {
                    cout << graveyardL.front() << " " << min(N,sizer) - p << endl;
                    graveyardL.pop_front();
                } 
                priority_queue<Zombie*, vector<Zombie*>, ZombLifeMost> most;
                for (uint32_t f = 0; f < field.size(); f++) {
                    most.push(&field[f]);
                }
                cout << "Most active zombies:\n";
                for (uint32_t g = 1; g <= N; g++) {
                    cout << most.top()->name << " " << most.top()->age << endl;
                    most.pop();
                }
                priority_queue<Zombie*, vector<Zombie*>, ZombLifeLeast> least;
                for (uint32_t f = 0; f < field.size(); f++) {
                    least.push(&field[f]);
                }
                cout << "Least active zombies:\n";
                for (uint32_t g = N; g >= 1; g--) {
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
            cin >> line >> numRandos; // random zombie pop
            cin >> line >> numOCs; // named zombie pop
            for (uint32_t q = 0; q < numRandos; q++) {
                Zombie temp = Zombie(false);
                field.push_back(temp);
                NUM_ZOMBIES++;
                if (verbose) cout << "Created: " << temp << 
                //" eta: " << temp.eta() <<
                "\n";
            } for (uint32_t s = 0; s < numOCs; s++) {
                Zombie temp = Zombie(true);
                field.push_back(temp);
                NUM_ZOMBIES++;
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
            Zombie* temp = shooting_range.top();
            shooting_range.pop();
            temp->damage();
            quiver--;
            //cout << "shooting: " << temp << endl;
            if (temp->die()) {
                if (shooting_range.empty()) win = true;
                if (verbose) {
                    cout << "Destroyed: " << *temp << endl;
                }
                lastKill = temp->name;
                if (statistics) {
                    if (graveyardF.size() < N) {
                        graveyardF.push_back(temp->name);
                        graveyardL.push_front(temp->name);
                    } else if (graveyardL.size() == N) {
                        //cout << "graveyard full\n";
                        graveyardL.pop_back();
                        graveyardL.push_front(temp->name);
                    }
                    if (firstkill) {
                        //cout << "FATALITY\n";
                        maxAge = minAge = temp->age;
                        firstkill = false;
                    } else {
                        if (temp->age > maxAge) {
                            maxAge = temp->age;
                        } if (temp->age < minAge) {
                            minAge = temp->age;
                        } 
                    }
                }
            } else {
                shooting_range.push(temp);
                //cout << temp << " survived \n";
            } //cout << field.size() << " zombies remaining \n";
        } 

        //cout << "sim exited\n";

        //cout << "step 7: median\n";
        if (median) {
            size_t iMedian = 0;
            uint32_t medVal = 0;
            priority_queue<uint32_t> ages;
            for (uint32_t p = 0; p < field.size(); p++) {
                if (!field[p].alive) ages.push(field[p].age);
            }
            if (ages.size()%2 == 1) {
                iMedian = ages.size()/size_t(2) - size_t(1);
                for (size_t w = 0; w < iMedian-1; w++) {
                    ages.pop();
                } uint32_t temp = ages.top();
                ages.pop();
                medVal = (ages.top() + temp)/uint32_t(2);
            } else {
                iMedian = (ages.size() - size_t(1))/size_t(2);
                for (size_t w = 0; w < iMedian-1; w++) {
                    ages.pop();
                } medVal = ages.top();
            }
            cout << "At the end of round " << round << ", the median zombie lifetime is " << medVal << endl;
        }
        
        // step 8: check win (while loop condition)
    }
    cout << "VICTORY IN ROUND " << round << "! " << lastKill << " was the last zombie.\n";
    // stats output
    if (statistics) {
        cout << "Zombies still active: 0\n";
        cout << "First zombies killed:\n";
        uint32_t sizer = uint32_t(graveyardF.size());
        for (uint32_t k = 1; k <= min(N,sizer); k++) {
            cout << graveyardF.front() << " " << k << endl;
            graveyardF.pop_front();
        } 
        cout << "Last zombies killed:\n";
        for (uint32_t p = 0; p < min(N,sizer); p++) {
            cout << graveyardL.front() << " " << min(N,sizer) - p << endl;
            graveyardL.pop_front();
        } 
        priority_queue<Zombie*, vector<Zombie*>, ZombLifeMost> most;
        for (uint32_t f = 0; f < field.size(); f++) {
            most.push(&field[f]);
        }
        cout << "Most active zombies:\n";
        for (uint32_t g = 1; g <= N; g++) {
            cout << most.top()->name << " " << most.top()->age << endl;
            most.pop();
        }
        priority_queue<Zombie*, vector<Zombie*>, ZombLifeLeast> least;
        for (uint32_t f = 0; f < field.size(); f++) {
            least.push(&field[f]);
        }
        cout << "Least active zombies:\n";
        for (uint32_t g = N; g >= 1; g--) {
            cout << least.top()->name << " " << least.top()->age << endl;
            least.pop();
        }
    }
}