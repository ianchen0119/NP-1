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

// ref: https://burweisnote.blogspot.com/2017/10/pipe.html

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
    int p = 0;
    char buf;
    if(pipe(this->pipefds[0]) != 0 || pipe(this->pipefds[1]) != 0){
        cout << "err!" << endl;
            for(;;){

            }
    }
    for(; i < this->cmdBlockCount; i++){

        // cout << "Block" << 0 << ".prev: "  << cmdBlockSet[i].prev << endl;
        // cout << "Block" << 0 << ".next: "  << cmdBlockSet[i].next << endl;
        this->parser(input, this->cmdBlockSet[i].start, this->cmdBlockSet[i].end);

        int j = 0;
        for(auto val: this->parse){
            this->execArg[j++] = strdup(val.c_str());
        }

        this->execArg[j] = NULL;

        this->parse.clear();


        /* wait for child */
        int pid = fork();

        if(pid){
            /* pipe */
            if(this->cmdBlockSet[i].prev == pipe_){
                close(this->pipefds[!p][1]);
            }

            if(this->cmdBlockSet[i].next == pipe_){
                p = !p;
            }
            /* file redict */

            /* wait for child */
            cout << "[init] child: " << pid << endl;
            wait(0); 
            cout << "[finished] child: " << pid << endl;

            if(this->cmdBlockSet[i].prev == pipe_){
                if(pipe(this->pipefds[p]) != 0){
                    cout << "err!" << endl;
                        for(;;){

                        }
                }
            }

        }else{
            if(this->cmdBlockSet[i].next == pipe_ && this->cmdBlockSet[i].prev == pipe_){
                close(this->pipefds[!p][1]);
                dup2(this->pipefds[!p][0], STDIN_FILENO);
                close(this->pipefds[!p][0]);
                close(this->pipefds[p][0]);
                dup2(this->pipefds[p][1] ,STDOUT_FILENO);
                close(this->pipefds[p][1]);
            }

            /* previous symbol is pipe */
            if(this->cmdBlockSet[i].prev == pipe_ && this->cmdBlockSet[i].next != pipe_){
                close(this->pipefds[p][0]);
                close(this->pipefds[p][1]);
                close(this->pipefds[!p][1]);
                dup2(this->pipefds[!p][0], STDIN_FILENO);
                close(this->pipefds[!p][0]);
            }
            /* next symbol is pipe */
            if(this->cmdBlockSet[i].next == pipe_ && this->cmdBlockSet[i].prev != pipe_){
                close(this->pipefds[!p][0]);
                close(this->pipefds[!p][1]);
                close(this->pipefds[p][0]);
                dup2(this->pipefds[p][1] ,STDOUT_FILENO);
                close(this->pipefds[p][1]);
            }


            /* next symbol is redict */

            /* exec */
            if(execvp(this->execArg[0], this->execArg)){
                    cerr << "Unknown command: [" << this->execArg[0] << "]" << endl;
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

        this->execCmd(input);
    }
}