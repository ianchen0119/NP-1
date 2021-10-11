#ifndef SH_H
#define SH_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef struct cmdBlock{
    /* prev symbol */
    int prev = 0;
    /* start pos */
    int start = 0;
    /* end pos */
    int end = 0;
    /* next symbol */
    int next = 0;
} cmdBlock;

class sh{
    private:
        int cmdBlockCount = 1;
        vector<string> parse;
        char* execArg[50] = {NULL};
        cmdBlock cmdBlockSet[10];
        int pipefds[2];
        int outfd;
        /* show prompt */
        void prompt();
        /* divide the cmd to cmdBlock(s) */
        void cmdBlockGen(string input);
        /* parse the cmdBlock */
        void parser(string input, int start, int end);
        /* exec the command */
        int execCmd(string input);
    public:
        sh() = default;
        ~sh() = default;
        void run();
};


#endif