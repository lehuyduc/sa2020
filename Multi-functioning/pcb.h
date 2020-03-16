#include <settings.h>
#include <map>
#include <vector>
#include <time.h>

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

    bool finished() {
        return counter==instructions.size();
    }

    void run1() {
        instructions[counter].run();
        counter++;
        mySleep(10); // pretends that each instruction cost 10ms
    }

    void runAll() {
        while (!finished()) run1();
    }

    void runForMs(int timeSlice) {
        int timePassed = 0;
        clock_t start = clock();
        while (timePassed < timeSlice && !finished()) {
            run1();
            timePassed = double(clock()-start) / (CLOCKS_PER_SEC / 1000);
        }
    }

    void clear() {
        counter = 0;
        varExist.clear();
        varInt.clear();
        instructions.clear();
    }


};
