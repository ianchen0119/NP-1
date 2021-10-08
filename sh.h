#ifndef SH_H
#define SH_H
#include <iostream>
#include <string>

#define empty 0
#define pipe 1
#define num_pipe 2
#define redir 3

typedef struct cmdBlock{
    /* prev symbol */
    int prev;
    /* start pos */
    int start;
    /* end pos */
    int end;
    /* next symbol */
    int next;
} cmdBlock;

class sh{
    private:
        /* show prompt */
        void prompt();
        /* divide the cmd to cmdBlock(s) */
        void cmdBlockGen();
        /* parse the cmdBlock */
        void parser();
        /* exec the command */
        int execCmd();
    public:
        sh() = default;
        ~sh() = default;
        void run();
}


#endif