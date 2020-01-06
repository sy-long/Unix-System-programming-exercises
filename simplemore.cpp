#include <stdio.h>
#include <stdlib.h>
#define MaxPageLine 24
#define MaxLineLen 512

int chiose(FILE* fp){
    int ch;
    printf("\033[7m more? \033[m");
    while((ch=getc(fp))!=EOF){
        switch (ch)
        {
        case ' ':return MaxPageLine;
            break;
        case '\n':return 1;
            break;
        case 'q':exit(0);
            break;
        default:
            break;
        }
    }
    if(ch==EOF) exit(0);
}

void run(FILE* fp){
    char line[512];
    int nowline=0;
    FILE *fp1=fopen("/dev/tty","r");
    int reply;
    while(fgets(line,MaxLineLen,fp)){
        if(nowline==MaxPageLine){
            reply=chiose(fp1);
            nowline-=reply;
        }
        if(fputs(line,stdout)==EOF) exit(1);
        nowline++;
    }
}

int main(int ac , char **av){
    FILE *fp;
    if(ac==1){
        run(stdin);
    }
    else{
        while(--ac){
            fp=fopen(*(++av),"r");
            if(fp==NULL) exit(1);
            run(fp);
            fclose(fp);
        }
    }
    return 0;
}