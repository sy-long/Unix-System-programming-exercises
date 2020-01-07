#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFSIZE 4096
#define COPYMODE 0644

void oops(char* s1,char* s2){
    fprintf(stderr,"Error:%s",s1);
    perror(s2);
    exit(1);
}

int main(int ac,char *av[]){
    if(ac!=3){
        fprintf(stderr,"usage: %s source destination\n",*av);
        exit(1);
    }
    int fd_in,fd_out,n_chars;
    char buf[BUFFSIZE];
    if((fd_in=(open(av[1],O_RDONLY)))==-1){
        oops("Cannot open",av[1]);
    }
    if((fd_out=creat(av[2],COPYMODE))==-1){
        oops("Cannot open",av[2]);
    }
    while((n_chars=read(fd_in,buf,BUFFSIZE))>0){
        if(write(fd_out,buf,n_chars)!=n_chars){
            oops("Write error to",av[2]);
        }
    }
    if(n_chars==-1){
        oops("Read error from",av[1]);
    }
    if(close(fd_in)==-1||close(fd_out)==-1){
        oops("Error closing files","");
    }
    return 0;
}