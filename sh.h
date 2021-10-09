#ifndef SH_H
#define SH_H
#include <iostream>
#include <string>

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
        /* show prompt */
        void prompt();
        /* divide the cmd to cmdBlock(s) */
        void cmdBlockGen(char input);
        /* parse the cmdBlock */
        void parser();
        /* exec the command */
        int execCmd();
        int cmdBlockCount = 1;
        cmdBlock cmdBlockSet[10];
        int pipefds[2];
        int outfd;
    public:
        sh() = default;
        ~sh() = default;
        void run();
}


#endif