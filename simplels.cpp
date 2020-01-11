#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

char* gid_to_name(gid_t gid){
    struct group* pgrp;
    static char numstr[10];

    if((pgrp=getgrgid(gid))==NULL){
        sprintf(numstr,"%d",gid);
        return numstr;
    }
    else{
        return pgrp->gr_name;
    }
}

char* uid_to_name(uid_t uid){
    struct passwd* ppw;
    static char numstr[10];

    if((ppw=getpwuid(uid))==NULL){
        sprintf(numstr,"%d",uid);
        return numstr;
    }
    else{
        return ppw->pw_name;
    }
}

void mode_to_letters(int mode,char *str){
    strcpy(str,"----------");
    if(S_ISDIR(mode)) str[0]='d';
    if(S_ISCHR(mode)) str[0]='c';
    if(S_ISBLK(mode)) str[0]='b';

    if(mode&S_IRUSR) str[1]='r';
    if(mode&S_IWUSR) str[2]='w';
    if(mode&S_IXUSR) str[3]='x';

    if(mode&S_IRGRP) str[4]='r';
    if(mode&S_IWGRP) str[5]='w';
    if(mode&S_IXGRP) str[6]='x';

    if(mode&S_IROTH) str[7]='r';
    if(mode&S_IWOTH) str[8]='w';
    if(mode&S_IXOTH) str[9]='x';
}

void show_file_info(char *filename,struct stat* pinfo){
    char modestr[11];
    mode_to_letters(pinfo->st_mode,modestr);
    printf("%s",modestr);
    printf("%4d",(int)pinfo->st_nlink);
    printf("%-8s",uid_to_name(pinfo->st_uid));
    printf("%-8s",gid_to_name(pinfo->st_gid));
    printf("%8ld",(long)pinfo->st_size);
    printf("%.12s",4+ctime(&pinfo->st_mtime));
    printf("%s\n",filename);
}

void do_stat(char filename[]){
    struct stat info;
    if(stat(filename,&info)==-1){
        perror(filename);
    }
    else{
        show_file_info(filename,&info);
    }
    
}

void do_ls(char dirname[]){
    DIR *pdir;
    struct dirent * pdirent;
    char fullname[255];
    int i;
    if((pdir=opendir(dirname))==NULL){
        fprintf(stderr,"simplels:cannot open %s\n",dirname);
    }
    else{
        while((pdirent=readdir(pdir))!=NULL){
            strcpy(fullname,dirname);
            for(i=0;fullname[i]!='\0';i++);
            fullname[i]='/';
            fullname[i+1]='\0';
            strcat(fullname,pdirent->d_name);
            do_stat(fullname);
        }
        closedir(pdir);
    }
}

int main(int ac,char *av[]){
    if(ac==1){
        do_ls(".");
    }
    else{
        while(--ac){
            printf("%s\n",*++av);
            do_ls(*av);
        }
    }
}