#include <settings.h>
#include <map>
#include <vector>

struct PCB {
    int pid;
    int priority;
    int counter;
    map<string, int> varExist;
    map<string, int*> varInt;
    vector<Instruction> instructions;

    PCB() {
        counter = 0;
    }

    void run() {
        while (counter < instructions.size()) {
            instructions[counter].run();
            counter++;
        }
    }

    void clear() {
        counter = 0;
       // varExist.clear();
       // varInt.clear();
       // instructions.clear();
    }
};
