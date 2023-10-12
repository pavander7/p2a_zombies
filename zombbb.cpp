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

    //placeholder variables for input processing
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

    uint32_t nextIndex = 0;
    
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
    uint32_t NUM_ZOMBIES = 0;
    deque<string> graveyardF;
    deque<string> graveyardL;
    uint32_t maxAge = 1;
    deque<string> tuffGuys;
    bool firstkill = true;
    uint32_t minAge = 1;
    deque<string> wimpyGuys;
    priority_queue<uint32_t> ages;

    //make zombie queue
    vector<Zombie> field;
    //priority_queue<Zombie, vector<Zombie>, ZombieCompare> field;

    //begin rounds
    getline(cin, line); // discard endl
    getline(cin, line); //discard delimiter
    uint32_t nextRound = 0;
    cin >> line >> nextRound; // round number
    while (!field.empty() || more) { 
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
                cout << "First zombies killed:\n";
                uint32_t sizer = uint32_t(graveyardF.size());
                for (uint32_t kvnt = 1; kvnt <= min(N,sizer); kvnt++) {
                    cout << graveyardF.front() << " " << kvnt << endl;
                    graveyardF.pop_front();
                } 
                cout << "Last zombies killed:\n";
                for (uint32_t pvssie = 0; pvssie < min(N,sizer); pvssie++) {
                    cout << graveyardL.front() << " " << min(N,sizer) - pvssie << endl;
                    graveyardL.pop_front();
                } 
                for (uint32_t uvula = 0; uvula < field.size(); uvula++) {
                    Zombie temp = field[uvula];
                    if (temp.alive) {
                        if (firstkill) {
                            //cout << "FATALITY\n";
                            maxAge = minAge = temp.age;
                            tuffGuys.push_back(temp.name);
                            wimpyGuys.push_back(temp.name);
                            firstkill = false;
                        } else {
                            if (temp.age > maxAge) {
                                maxAge = temp.age;
                                tuffGuys.clear();
                                tuffGuys.push_back(temp.name);
                            } else if (temp.age == maxAge) {
                                tuffGuys.push_back(temp.name);
                            } if (temp.age < minAge) {
                                minAge = temp.age;
                                wimpyGuys.clear();
                                wimpyGuys.push_back(temp.name);
                            } else if (temp.age == minAge) {
                                wimpyGuys.push_back(temp.name);
                            }
                        }
                    }
                }
                sort(tuffGuys.begin(), tuffGuys.end());
                sort(wimpyGuys.begin(),wimpyGuys.end());
                cout << "Most active zombies:\n";
                sizer = uint32_t(tuffGuys.size());
                for (uint32_t kawk = 1; kawk <= min(N,sizer); kawk++) {
                    cout << tuffGuys.front() << " " << maxAge << endl;
                    tuffGuys.pop_front();
                }
                cout << "Least active zombies:\n";
                sizer = uint32_t(wimpyGuys.size());
                for (uint32_t bawls = 1; bawls <= min(N,sizer); bawls++) {
                    cout << wimpyGuys.front() << " " << minAge << endl;
                    wimpyGuys.pop_front();
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
                Zombie temp = Zombie(false, nextIndex);
                nextIndex++;
                field.push_back(temp);
                NUM_ZOMBIES++;
                if (verbose) cout << "Created: " << temp << 
                //" eta: " << temp.eta() <<
                "\n";
            } for (uint32_t s = 0; s < numOCs; s++) {
                Zombie temp = Zombie(true, nextIndex);
                nextIndex++;
                field.push_back(temp);
                NUM_ZOMBIES++;
                if (verbose) cout << "Created: " << temp <<
                //" eta: " << temp.eta() <<
                "\n";
            }
            getline(cin, line); //discard delimiter
            if (!(cin >> line >> nextRound)) { // round number
                more = false;
                //cout << "no more waves \n";
            }
        }

        //cout << "step 6: player shoots zombies\n";
        priority_queue<Zombie, vector<Zombie>, ZombieCompare> shooting_range;
        for (uint32_t kvm = 0; kvm < field.size(); kvm++) {
            if (field[kvm].alive) shooting_range.push(field[kvm]);
        }
        while (quiver != 0) {
            Zombie temp = shooting_range.top();
            shooting_range.pop();
            temp.damage();
            quiver--;
            //cout << "shooting: " << temp << endl;
            if (temp.die()) {
                if (verbose) {
                    cout << "Destroyed: " << temp << endl;
                }
                lastKill = temp.name;
                if (graveyardF.size() < N) {
                    graveyardF.push_back(temp.name);
                    graveyardL.push_front(temp.name);
                } else if (graveyardL.size() == N) {
                    //cout << "graveyard full\n";
                    graveyardL.pop_back();
                    graveyardL.push_front(temp.name);
                }
                if (statistics) {
                    if (firstkill) {
                        //cout << "FATALITY\n";
                        maxAge = minAge = temp.age;
                        tuffGuys.push_back(temp.name);
                        wimpyGuys.push_back(temp.name);
                        firstkill = false;
                    } else {
                        if (temp.age > maxAge) {
                            maxAge = temp.age;
                            tuffGuys.clear();
                            tuffGuys.push_back(temp.name);
                        } else if (temp.age == maxAge) {
                            tuffGuys.push_back(temp.name);
                        } if (temp.age < minAge && !firstkill) {
                            minAge = temp.age;
                            wimpyGuys.clear();
                            wimpyGuys.push_back(temp.name);
                        } else if (temp.age == minAge) {
                            wimpyGuys.push_back(temp.name);
                        }
                    }
                } if (median) ages.push(temp.age);
            } else {
                shooting_range.push(temp);
                //cout << temp << " survived \n";
            } //cout << field.size() << " zombies remaining \n";
            field[temp.index] = temp;
        }

        //cout << "sim exited\n";

        //cout << "step 7: median\n";
        if (median) {
            size_t iMedian = 0;
            uint32_t medVal = 0;
            priority_queue<uint32_t> bufferAges = ages;
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
            ages = bufferAges;
            cout << "At the end of round " << round << ", the median zombie lifetime is " << medVal << endl;
        }
        
        // step 8: check win (while loop condition)
    }
    cout << "VICTORY IN ROUND " << round << "! " << lastKill << " was the last zombie.\n";
    // stats output
    if (statistics) {
        cout << "First zombies killed:\n";
        for (uint32_t kvnt = 1; kvnt <= min(size_t(N),graveyardF.size()); kvnt++) {
            cout << graveyardF.front() << " " << kvnt << endl;
            graveyardF.pop_front();
        } 
        cout << "Last zombies killed:\n";
        for (uint32_t pvssie = 1; pvssie <= min(size_t(N),graveyardL.size()); pvssie++) {
            cout << graveyardL.front() << " " << pvssie << endl;
            graveyardL.pop_front();
        } 
        cout << "Most active zombies:\n";
        for (uint32_t kawk = 1; kawk <= min(size_t(N),tuffGuys.size()); kawk++) {
            cout << tuffGuys.front() << " " << maxAge << endl;
            tuffGuys.pop_back();
        }
        cout << "Least active zombies:\n";
        for (uint32_t bawls = 1; bawls <= min(size_t(N),wimpyGuys.size()); bawls++) {
            cout << wimpyGuys.front() << " " << minAge << endl;
            wimpyGuys.pop_back();
        }
    }
}
