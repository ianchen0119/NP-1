#include "sh.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

#define empty_ 0
#define pipe_ 1
#define num_pipe_ 2
#define redirOut_ 3
#define redirIn_ 4


void sh::prompt(){
    /* Data reset */
    this->cmdBlockCount = 1;
    cout << "# ";
}

void sh::cmdBlockGen(string input){
    int count = 0;
    int prevSymbol = empty_;
    this->cmdBlockSet[this->cmdBlockCount - 1].start = count;
    while(input[count] != '\0'){
        switch(input[count]){
            case '|':
                // ...
                this->cmdBlockSet[this->cmdBlockCount - 1].prev = prevSymbol;
                prevSymbol = pipe_;
                this->cmdBlockSet[this->cmdBlockCount - 1].next = prevSymbol;
                this->cmdBlockSet[this->cmdBlockCount - 1].end = count - 1;
                this->cmdBlockCount++;
                // next cmdBlock
                this->cmdBlockSet[this->cmdBlockCount - 1].start = count + 1;
                break;
            case '>':
                // ...
                this->cmdBlockSet[this->cmdBlockCount - 1].prev = prevSymbol;
                prevSymbol = redirOut_;
                this->cmdBlockSet[this->cmdBlockCount - 1].next = prevSymbol;
                this->cmdBlockSet[this->cmdBlockCount - 1].end = count - 1;
                this->cmdBlockCount++;
                // next cmdBlock
                this->cmdBlockSet[this->cmdBlockCount - 1].start = count + 1;
                break;
            case '<':
                 // ...
                this->cmdBlockSet[this->cmdBlockCount - 1].prev = prevSymbol;
                prevSymbol = redirIn_;
                this->cmdBlockSet[this->cmdBlockCount - 1].next = prevSymbol;
                this->cmdBlockSet[this->cmdBlockCount - 1].end = count - 1;
                this->cmdBlockCount++;
                // next cmdBlock
                this->cmdBlockSet[this->cmdBlockCount - 1].start = count + 1;
                break;
            default:
                // ...
                break;
        }
        count++;
    }
    this->cmdBlockSet[this->cmdBlockCount - 1].prev = prevSymbol;
    this->cmdBlockSet[this->cmdBlockCount - 1].next = empty_;
    this->cmdBlockSet[this->cmdBlockCount - 1].end = count;
}

void sh::parser(string input, int start, int end){
    string temp;
    for(int i = start; i < end; i++){
        if(input[i]==' '){
            if(!temp.empty()){
                this->parse.push_back(temp);
                temp.clear();
            }
            continue;
	    }
        temp.push_back(input[i]);
    }
    this->parse.push_back(temp);	
}

int sh::execCmd(string input){
    int i = 0;
    for(; i < this->cmdBlockCount; i++){
        this->parser(input, this->cmdBlockSet[i].start, this->cmdBlockSet[i].end);

        int j = 0;
        for(auto val: this->parse){
            this->execArg[j++] = strdup(val.c_str());
        }

        this->execArg[j] = NULL;

        this->parse.clear();

        /* prepare to fork */
        if(this->cmdBlockSet[i].next == pipe_){
            close(this->pipefds[0]);
            pipe(this->pipefds);
            this->outfd = this->pipefds[0];
        }

        /* wait for child */
        int pid = fork();

        if(pid){
            /* pipe */
            if(this->cmdBlockSet[i].next == pipe_){
                close(this->pipefds[1]);
            }
            if(this->cmdBlockSet[i].prev == pipe_){
                close(this->outfd);
            } 
            /* file redict */

            /* wait for child */
            wait(0); 
        }else{
            /* previous symbol is pipe */
            if(this->cmdBlockSet[i].prev == pipe_){
                dup2(this->outfd, 0);
                close(this->outfd);
            }
            /* next symbol is pipe */
            if(this->cmdBlockSet[i].next == pipe_){
                dup2(this->pipefds[1] ,1);
                close(this->pipefds[1]);
                close(this->pipefds[0]);
            }
            /* next symbol is redict */
            if(execvp(this->execArg[0], this->execArg)){
                    cerr << "unknown cmd: " << this->execArg[0] << endl;
                    exit(0);
            }
        }

    }
    return 0;
}

void sh::run(){

    string input;

    while(true){
        this->prompt();

        getline(cin, input);

        if(input.length() == 0){
            continue;
        }

        this->cmdBlockGen(input);

        // cout << "next: " << this->cmdBlockSet[0].next << endl;
        // cout << "prev: " << this->cmdBlockSet[0].prev << endl;
        // cout << "start: " << this->cmdBlockSet[0].start << endl;
        // cout << "end: " << this->cmdBlockSet[0].end << endl;

        this->execCmd(input);
    }
}