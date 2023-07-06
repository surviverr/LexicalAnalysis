#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
#include <set>
#include "Graph.hpp"

using namespace std;

const int maxn = 1e4 + 5;

int char2num(char ch) {
    if(ch >= 'a' && ch <= 'z') return ch - 'a' + 1;
    if(ch >= 'A' && ch <= 'Z') return ch - 'A' + 1;
    if(ch >= '0' && ch <= '9') return ch - '0' + 27;
    if(ch == '+') return 37;
    if(ch == '-') return 38;
    if(ch == '*') return 39;
    if(ch == '/') return 40;
    if(ch == '%') return 41;
    if(ch == '>') return 42;
    if(ch == '<') return 43;
    if(ch == '=') return 44;
    if(ch == '!') return 45;
    if(ch == '&') return 46;
    if(ch == '|') return 47;
    if(ch == '(') return 48;
    if(ch == ')') return 49;
    if(ch == '{') return 50;
    if(ch == '}') return 51;
    if(ch == ';') return 52;
    if(ch == '_') return 53;
    if(ch == ',') return 54;
    return -1;
}

char ch[maxn];

stack<char> toRead, lexCache;
Graph NFA, DFA, miniDFA;
int now, suc;

void input(char *inputFilePath) {
    ifstream inFile(inputFilePath, ios::in | ios::binary);
    inFile.read((char *)&ch, sizeof(ch));
    int len = strlen(ch);
    for(int i = len - 1;i >= 0;i--) toRead.push(ch[i]);
}

void PrintNow() {
    stack<char> tmp;
    while(!lexCache.empty()) {
        tmp.push(lexCache.top());
        lexCache.pop();
    }
    while(!tmp.empty()) {
        printf("%c", tmp.top());
        tmp.pop();
    }
    printf("\t");
    if(suc == 0) printf("<IDN>\n");
    else if(suc == 1) printf("<OP>\n");
    else if(suc == 2) printf("<KW>\n");
    else if(suc == 3) printf("<INT>\n");
    else printf("<SE>\n");
    suc = -1;
    now = 0;
}

void solve(char *outFilePath) {
    freopen(outFilePath, "w", stdout);
    NFA.makeNFA();
    DFA = NFA.makeDFA();
    miniDFA = DFA.minimizeDFA();
    now = 0, suc = -1;
    while(!toRead.empty()) {
        if(char2num(toRead.top()) == -1) {
            toRead.pop();
            if(!lexCache.empty()) PrintNow();
        } else {
            now = miniDFA.getNext(now, char2num(toRead.top()));
            if(miniDFA.wordType[now] == -1 && suc >= 0) {
                PrintNow();
            } else {
                lexCache.push(toRead.top());
                suc = miniDFA.wordType[now];
                toRead.pop();
            }
        }
    }
    if(suc != -1) PrintNow();
}

int main(int argc, char **argv) {
    input(argv[1]), solve(argv[2]);
    return 0;
}