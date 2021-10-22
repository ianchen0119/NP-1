#include "sh.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_chld(int signo){
    int status;
    wait(&status);
    return;
}

int main(){
  void sig_chld(int);
  signal(SIGCHLD, sig_chld);

  sh instance;
  
  instance.run();
  return 0;
}