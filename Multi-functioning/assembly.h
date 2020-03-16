#include <string>
#include <vector>
#include <iostream>
using namespace std;

typedef void (*InstructionFunc)(vector<int*>&, string&);

const int MOVID = 0;

struct Instruction {
    string insName;
    InstructionFunc func;
    vector<int*> args;
    string argStr;

    Instruction(InstructionFunc newFunc, vector<int*> newArgs, string newStr) {
        func = newFunc;
        args = newArgs;
        argStr = newStr;
    }

    Instruction(InstructionFunc newFunc, vector<int*> newArgs, string newStr, string name) {
        func = newFunc;
        args = newArgs;
        argStr = newStr;
        insName = name;
    }

    void run() {
        func(args, argStr);
    }
};

void SET(vector<int*>& args, string& str) {
    int *to = args[0], *from = args[1];
  //  cout << "set1 " << to << " " << from << " " << (*to) << " " << (*from) << "\n";
    (*to) = (*from);
   // cout << "set2 " << to << " " << from << " " << (*to) << " " << (*from) << "\n";
}

//-------------------       MATH INSTRUCTION

void INC(vector<int*>& args, string& str) {
    (*args[0]) += 1;
}

void DEC(vector<int*>& args, string& str) {
    (*args[0]) -= 1;
}

void ADD(vector<int*>& args, string& str) {
    int *to = args[0], *from = args[1];
    (*to) = (*to) + (*from);
}

void SUB(vector<int*>& args, string& str) {
    int *to = args[0], *from = args[1];
    //cout << "sub1 " << to << " " << from << " " << (*to) << " " << (*from) << "\n";
    (*to) = (*to) - (*from);
    //cout << "sub2 " << to << " " << from << " " << (*to) << " " << (*from) << "\n\n";
}

void MUL(vector<int*>& args, string& str) {
    int *to = args[0], *from = args[1];
    (*to) = (*to) * (*from);
}

void DIV(vector<int*>& args, string& str) {
    int *to = args[0], *from = args[1];
    (*to) = (*to) / (*from);
}

void MOD(vector<int*>& args, string& str) {
    int *to = args[0], *from = args[1];
    (*to) = (*to) % (*from);
}

//-------------------------  COMPARISON INSTRUCTION

void CMPE(vector<int*>& args, string& str) {
    int *to = args[0], *a = args[1], *b = args[2];
    (*to) = (*a)==(*b);
}

void CMPL(vector<int*>& args, string& str) {
    int *to = args[0], *a = args[1], *b = args[2];
    (*to) = (*a) < (*b);
}

void CMPLE(vector<int*>& args, string& str) {
    int *to = args[0], *a = args[1], *b = args[2];
    (*to) = (*a) <= (*b);
}

void JUMPIF(vector<int*>& args, string& str) {
    int *counter = args[0], *destination = args[1], *condition = args[2];
    if (*condition==true) *counter = *destination;
}

void LABEL(vector<int*>& args, string& str) {
    // do nothing, it's just a label for JUMP
}

//-----------------         ARRAY INSTRUCTION
//
void SETA(vector<int*>& args, string& str) {
    int *arr = args[0], *pos = args[1], *from = args[2];
    arr[*pos] = *from;
  //  cout << "seta: " << arr << " " << (*pos) << " " << (*from) << "\n";
}

void GETA(vector<int*>& args, string& str) {
    int *to = args[0], *arr = args[1], *pos = args[2];
    *to = arr[*pos];
    //cout << "geta: " << (to) << " " << arr << " " << (*pos) << "\n";
}

// INPUT INSTRUCTION


//  OUTPUT INSTRUCTION
void PRINTINT(vector<int*>& args, string& str) {
    printf("%d ",*(args[0]));
    fflush(stdout);
}

void PRINTLN(vector<int*>& args, string& str) {
    printf("\n");
    fflush(stdout);
}

InstructionFunc name2ins(string s)
{
    if (s=="INC") return &INC;
    else if (s=="DEC") return &DEC;
    else if (s=="ADD") return &ADD;
    else if (s=="SUB") return &SUB;
    else if (s=="MUL") return &MUL;
    else if (s=="DIV") return &DIV;
    else if (s=="MOD") return &MOD;
    else if (s=="SET") return &SET;
    else if (s=="CMPE") return &CMPE;
    else if (s=="CMPL") return &CMPL;
    else if (s=="CMPLE") return &CMPLE;
    else if (s=="JUMPIF") return &JUMPIF;
    else if (s=="LABEL") return &LABEL;
    else if (s=="SETA") return &SETA;
    else if (s=="GETA") return &GETA;
    else if (s=="PRINTINT") return &PRINTINT;
    else if (s=="PRINTLN") return &PRINTLN;

    return 0;
    cout << "crash because: " << s << "\n";
    throw("unknown name");
}

/*
void* MALLOC(int bytes) {
    return malloc(bytes);
}


// move variable OR constant
void MOV(void *to, void *from, int n) {
    memcpy(to, from, n);
}

// move constant: src = dest
void MOVC(void *to, void *from, int type) {
    if (type==INT) {
        memcpy(to, from, sizeof(int));
    }
    else if (type==FLOAT) {
        memcpy(to, from, sizeof(float));
    }
    else {
        memcpy(to, from, strlen((char*)from));
    }
}
void INC(void* a, int type) {
    int tint;
    float tfloat;
    if (type==INT) {
        tint = (int*)(a) + 1;
        MOV(a, tint, sizeof(int));
    }
    else if (tye==FLOAT) {
        tfloat = (float*)(a) + 1;
        MOV(a, tfloat, sizeof(float));
    }
}

void ADD(void* a, void* b, int dtype) {
    int tint;
    float tfloat;
    if (type==INT) {
        tint = (int*)(a) + (int*)(b);
        memcpy(a, tint, sizeof(int));
    }
    else if (tye==FLOAT) {
        tfloat = (float*)(a) + (float*)(b);
        memcpy(a, tfloat, sizeof(float));
    }
}

void SUB(void* a, void* b, int dtype) {
    int tint;
    float tfloat;
    if (type==INT) {
        tint = (int*)(a) - (int*)(b);
        memcpy(a, tint, sizeof(int));
    }
    else if (tye==FLOAT) {
        tfloat = (float*)(a) - (float*)(b);
        memcpy(a, tfloat, sizeof(float));
    }
}
*/
