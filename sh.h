#ifndef SH_H
#define SH_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef struct cmdBlock{
    /* numbered pipe */
    int8_t num;
    /* prev symbol */
    int8_t prev = 0;
    /* next symbol */
    int8_t next = 0;
    /* start pos */
    int start = 0;
    /* end pos */
    int end = 0;
} cmdBlock;

class sh{
    private:
        /* Block counter */
        int cmdBlockCount = 1;
        /* Store the result of parsed cmdBlock */
        vector<string> parse;
        char* execArg[50] = {NULL};
        /* Can handle 1000 sub-commands at once */
        cmdBlock cmdBlockSet[2000];
        int pipefds[2][2];
        int numPipefds[10][2];
        int numpCount = 0;
        /* For numbered pipe */
        int timerArr[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
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