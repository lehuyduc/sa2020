#include <utils.h>
#include <assembly.h>
#include <compiler.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

void roundrobin()
{

}

int main()
{
    PCB pcb;
    Compiler compiler;

    compiler.compileFile("fibonacci.as",&pcb);

    cout << "\n";
    map<string, int*> varInt = pcb.varInt;
    map<string, int*>::iterator it;
    varInt = pcb.varInt;
    for (it = varInt.begin(); it!=varInt.end(); it++) {
        cout << (it->first) << " " << (it->second) << '\n';
    }
    cout << "Done compiling \n\n";

    pcb.run();

    compiler.compileFile("fibonacci2.as",&pcb);
    pcb.run();

    //cout << "Hello world!" << endl;
    return 0;
}
