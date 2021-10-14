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
        /* Block counter */
        int cmdBlockCount = 1;
        /* Store the result of parsed cmdBlock */
        vector<string> parse;
        char* execArg[50] = {NULL};
        /* Can handle 1000 sub-commands at once */
        cmdBlock cmdBlockSet[1000];
        int pipefds[3][2];
        /* For numbered pipe */
        int timer = 0;
        /* Show prompt */
        void prompt();
        /* Divide the cmd to cmdBlock(s) */
        void cmdBlockGen(string input);
        /* Parse the cmdBlock */
        void parser(string input, int start, int end);
        /* Exec the command */
        int execCmd(string input);
    public:
        sh() = default;
        ~sh() = default;
        void run();
};


#endif