#pragma once

#ifdef __linux_
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include <settings.h>
#include <string>
#include <time.h>
using namespace std;

void mySleep(int sleepMs)
{
#ifdef __linux_
    usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef _WIN32
    Sleep(sleepMs);
#endif
}


int string2int(string s)
{
    int start, n, res, negative;

    n = s.length();
    res = 0;
    negative = s[0]=='-';
    if (negative) start = 1; else start = 0;

    for (int i=start; i<n; i++) {
        res = res*10 + s[i]-48;
    }
    if (!negative) return res;
    else return -res;
}

bool isInteger(string s) {
    int n = s.length();
    for (int i=0; i<n; i++)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

