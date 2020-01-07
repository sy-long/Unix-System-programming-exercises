#include <stdio.h>
#include <utmp.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define SHOWHOST

void showtime(long timeval){
    char *cp;
    cp=ctime(&timeval);
    printf("%12.12s",cp+4);
}

void show_info(struct utmp *putbuf){
	if(putbuf->ut_type!=USER_PROCESS)
        return;
        printf("% -8.8s",putbuf->ut_name);
        printf(" ");
        printf("% -8.8s",putbuf->ut_line);
        printf(" ");
        showtime(putbuf->ut_time);
	#ifdef SHOWHOST
		if(putbuf->ut_host[0]!='\0')
			printf("(%s)",putbuf->ut_host);
	#endif
	printf("\n");
}

int main(){
    int utmpfd;
    if((utmpfd=open(UTMP_FILE,O_RDONLY))==-1){
        perror("UTMP_FILE");
        exit(0);
    }
    struct utmp utbuf;
    while(read(utmpfd,&utbuf,sizeof(utbuf))==sizeof(utbuf)){
        show_info(&utbuf);
    }
    close(utmpfd);
    return 0;
}
