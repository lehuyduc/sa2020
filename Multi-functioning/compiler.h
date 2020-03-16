#include <settings.h>
#include <string>
#include <map>
#include <pcb.h>
#include <fstream>

struct Compiler {
    //---------------   BASIC STRING PREPROCESSING
    bool invalidChar(char c) {
        if (c==' ' || c==',' || c==':') return true;
        return false;
    }

    void normalize(string &s) {
        int commentPos = s.find("//");
        if (commentPos != string::npos) s = s.substr(0, commentPos);

        while (s.length() > 0 && invalidChar(s[s.length()-1])) s.erase(s.length()-1, 1);
        while (s.length() > 0 && invalidChar(s[0])) s.erase(0, 1);

        // make the first word uppercase, because instruction should be uppercase
    }

    // pos point to the ' ' character, or the end of string s[s.length()]
    string getword(const string& s, int &pos) {
        int from = pos;
        int n = s.length();

        while (from<n && invalidChar(s[from])) from++;
        pos = from;

        while (pos < n && s[pos]!=' ') pos++;
        return s.substr(from, pos-from);
    }

    int* getIntVar(string s, PCB *pcb) {
        int *res;
        if (isInteger(s)) {
            res = new int;
            *res = string2int(s);
        }
        else {
            res = pcb->varInt[s];
        }
        return res;
    }

    //--------------- STRING PROCESSING FOR ARRAY VARIABLE
    int getArrayVarLength(string s) {
        int i = 0, j, n;
        n = s.length();
        while (s[i]!='[') i++;
        i++;
        j = i;
        while (s[j]!=']') j++;
        return string2int(s.substr(i, j-i));
    }

    string getArrayVarName(string s) {
        int i = 0;
        while (s[i]!='[') i++;
        return s.substr(0, i);
    }

    // read the text file and make sure it's in right format
    vector<string> file2text(string filename)
    {
        vector<string> lines;
        string s;
        ifstream fin(filename);

        while (getline(fin, s)) {
            normalize(s);
            if (s.length() > 2)
                lines.push_back(s);
        }
        return lines;
    }

    void parseVar(vector<string> lines, PCB *pcb) {
        int i, j, t, n, nLine, len;
        string line, varName, typeStr, s;
        map<string, int> varExist;
        map<string, int*> varInt;

        nLine = lines.size();
        t = 0;
        for (t=1; t<nLine;t++) {
            line = lines[t];
            if (line.find("text") != string::npos) break;

            n = line.length();
            i = 0;
            typeStr = getword(line, i);
            i++;

            if (typeStr=="int") {
                while (i < n) {
                    varName = getword(line, i);
                    varExist[varName] = 1;
                    varInt[varName] = new int;
                    *varInt[varName] = 0;
                    i++;
                }
            }
            else
            if (typeStr.find("int[") != string::npos) {
                while (i < n) {
                    s = getword(line, i);
                    varName = getArrayVarName(s);
                    len = getArrayVarLength(s);
                    varExist[varName] = 1;
                    varInt[varName] = new int[len];
                    for (int k=0; k<len; k++) varInt[varName][k] = 0;
                    i++;
                }
            }
        }

        pcb->varExist = varExist;
        pcb->varInt = varInt;
    }


    //-----------------------------------------------------------------------------------
    void parseCode(vector<string> lines, PCB *pcb) {
        int i, j, t, n, nLine,  len;
        string line, insName, s, arg, argStr;
        string jumpto, coondition;

        map<string, int> varExist = pcb->varExist;
        map<string, int*> varInt = pcb->varInt;
        vector<Instruction> instructions;
        vector<int*> args;

        nLine = lines.size();
        j = 0;
        while (lines[j].find("text") == string::npos) j++;
        j++;


        // pre loop to find LABEL positions first
        int insCount = 0;
        for (t=j; t<nLine; t++) {
            insCount++;
            line = lines[t];

            i = 0;
            insName = getword(line, i);
            if (insName=="LABEL") {
                s = getword(line, i);
                varInt[s] = new int;
                *varInt[s] = insCount - 1; // we start counting from 0
            }
        }

        for (t=j; t<nLine; t++ ) {
            line = lines[t];
            if (debug) cout << line << "\n";

            args.clear();
            argStr = "";
            n = line.length();

            i = 0;
            insName = getword(line, i);
            i++;

            if (insName=="JUMPIF") {
                string jumpto = getword(line, i);
                string condition = getword(line, i);

                args.push_back(&pcb->counter);
                args.push_back(varInt[jumpto]);
                args.push_back(varInt[condition]);
                instructions.push_back(Instruction(name2ins(insName), args, argStr, insName));
                continue;
            }

            while (i < n) {
                arg = getword(line, i);

                i++;
                if (isInteger(arg)) {
                    int *c = new int;
                    (*c) = string2int(arg);
                    args.push_back(c);
                }
                else {
                    args.push_back(varInt[arg]);
                }
            }
            instructions.push_back(Instruction(name2ins(insName), args, argStr, insName));
        }

        pcb->varExist = varExist;
        pcb->varInt = varInt;
        pcb->instructions = instructions;
    }

    void compileFile(string filename, PCB *pcb) {
        pcb->clear();
        vector<string> lines = file2text(filename);
        parseVar(lines, pcb);
        parseCode(lines, pcb);
    }
};
