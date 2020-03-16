#include <utils.h>
#include <assembly.h>
#include <compiler.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <deque>
using namespace std;

void fcfs(deque<PCB*> pcbs) {
    PCB *pcb;
    while (!pcbs.empty()) {
        pcb = pcbs.front(); pcbs.pop_front();
        pcb->runAll();
    }
}

void roundrobin(deque<PCB*> pcbs, int timeSlice)
{
    PCB *pcb;
    while (!pcbs.empty()) {
        pcb = pcbs.front(); pcbs.pop_front();
        pcb->runForMs(timeSlice);
        if (!pcb->finished()) pcbs.push_back(pcb);
    }
}

int main()
{
    deque<PCB*> pcbs;
    PCB pcb1, pcb2, pcb3;
    Compiler compiler;

    compiler.compileFile("fibonacci.as", &pcb1);
    compiler.compileFile("gcd.as",&pcb2);
    compiler.compileFile("fibonacci2.as",&pcb3);

    cout << "\n\n***************TESTING FIRST-COME FIRST-SERVE***************\n\n";
    pcbs.push_back(&pcb1);
    pcbs.push_back(&pcb2);
    pcbs.push_back(&pcb3);
    fcfs(pcbs);

    cout << "\n\n***************TESTING ROUND ROBIN***************\n\n";
    compiler.compileFile("fibonacci.as", &pcb1);
    compiler.compileFile("gcd.as",&pcb2);
    compiler.compileFile("fibonacci2.as",&pcb3);

    pcbs.push_back(&pcb1);
    pcbs.push_back(&pcb2);
    pcbs.push_back(&pcb3);
    roundrobin(pcbs, 50);

    //cout << "Hello world!" << endl;
    return 0;
}
