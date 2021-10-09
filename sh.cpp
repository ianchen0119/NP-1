#include "sh.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

#define empty 0
#define pipe 1
#define num_pipe 2
#define redirOut 3
#define redirIn 4


void sh::prompt(){
    /* Data reset */
    this->cmdBlockCount = 1;
    cout << "# " << endl;
}

void sh::cmdBlockGen(char input){
    int count = 0;
    int prevSymbol = empty;
    while(*(input + count) != '\0'){
        switch(*(input + count)){
            case '|':
                // ...
                this->cmdBlockSet[this->cmdBlockCount - 1].prev = prevSymbol;
                prevSymbol = pipe;
                this->cmdBlockSet[this->cmdBlockCount - 1].next = prevSymbol;
                this->cmdBlockSet[this->cmdBlockCount - 1].end = count - 1;
                this->cmdBlockCount++;
                // next cmdBlock
                this->cmdBlockSet[this->cmdBlockCount - 1].start = count + 1;
                break;
            case '>':
                // ...
                this->cmdBlockSet[this->cmdBlockCount - 1].prev = prevSymbol;
                prevSymbol = redirOut;
                this->cmdBlockSet[this->cmdBlockCount - 1].next = prevSymbol;
                this->cmdBlockSet[this->cmdBlockCount - 1].end = count - 1;
                this->cmdBlockCount++;
                // next cmdBlock
                this->cmdBlockSet[this->cmdBlockCount - 1].start = count + 1;
                break;
            case '<':
                 // ...
                this->cmdBlockSet[this->cmdBlockCount - 1].prev = prevSymbol;
                prevSymbol = redirIn;
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
}

void sh::parser(){
    
}

int sh::execCmd(){
    
}

void sh::run(){

    char input[30];

    while(true){
        this->prompt();

        getline(cin, input);

        if(input.length() == 0){
            continue;
        }

        this->cmdBlockGen(input);

        this->execCmd();
    }
}