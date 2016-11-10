#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define FALSE 0
#define TRUE !FALSE



void do_ls(char []);

char *filemode(mode_t); 



main(int ac, char *av[],char *mode)
{
 if(ac == 1 )
  do_ls(".");
  else
    while( --ac ){
    
    printf("%s:\n", *++av );
     do_ls( *av );

     
     
    }}


void do_ls( char dirname[] )
 {
     DIR *dir_ptr;
     struct dirent  *direntp;
     struct stat statbuf;
     struct passwd  *pwd;
     struct group   *grp;
     struct tm      *tm;
     char            datestring[256];
     char name[256];
     int  i=0 ;
     float temp=0;

    if(( dir_ptr = opendir( dirname )) == NULL)      
        fprintf(stderr, "lsl: cannot open %s\n", dirname);
      else
     {
            
       while(( direntp = readdir( dir_ptr)) != NULL ) 
          {
           

            if (stat(direntp->d_name, &statbuf) == -1)
                  continue;
 
             name[0]=direntp->d_name[0];
           //if(strcmp(direntp->d_name,".") && (strcmp(direntp->d_name,"..")))
             if(name[0] != '.')
           {           

            char *str = filemode(statbuf.st_mode);
             
                if (direntp->d_name != ".")
                  
          {
             
             int j= (int)statbuf.st_nlink;
             printf("%10.10s", str); 
             printf("%4d", j);

      if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
        printf(" %-1.8s", pwd->pw_name);
       else
        printf(" %-1d", statbuf.st_uid);

      if ((grp = getgrgid(statbuf.st_gid)) != NULL)
        printf(" %-4.8s", grp->gr_name);
        else
        printf(" %-4d", statbuf.st_gid);

   
          temp=statbuf.st_size;
          i=i+(ceil(temp/4096))*4;
                   
     
    
     printf("%5jd", (intmax_t)temp);


    tm = localtime(&statbuf.st_mtime);
    strftime(datestring, sizeof(datestring), "%b %d %H:%M" , tm);

   
    printf(" %s %s\n", datestring, direntp->d_name);  
       

   
      }   
       }
}
    printf("Total %d	\n",i);
       closedir(dir_ptr);  
   }
       
 }


 char * filemode(mode_t mode )
/*
* returns string of mode info
* default to ------- and then turn on bits
*/
{
static char bits[11];
char type;
struct stat sb;
strcpy( bits, "----------" );

switch ( mode & S_IFMT ){
 /* mask for type */
case S_IFREG: 
type = '-'; 
break;
 /* stays a dash*/
case S_IFDIR:
 type = 'd';
 
 break;
 /* put a d there*/
case S_IFCHR:
 type = 'c';
 
 break;
 /* char i/o dev*/
case S_IFBLK:
 type = 'b';
 break;
 /* blk. i/o dev*/
default: 
type = '?'; 
break;
 /* fifo, socket..*/
}
bits[0] = type ;


/* do SUID, SGID, and SVTX later */
permbits( mode>>6 , bits+1 ); /* owner */
permbits( mode>>3 , bits+4 ); /* group */
permbits( mode , bits+7 ); /* world */

return bits;
}

permbits( permval, string )
char *string;
/*
* convert bits in permval into chars rw and x
*/
{


if ( permval & 4 )
{
string[0] = 'r';

}
if ( permval & 2 )
{
string[1] = 'w';

}
if ( permval & 1 )
{
string[2] = 'x';

}
}


char *uid_to_name( uid )
short uid;
/*
* returns pointer to logname associated with uid, uses getpw()
*/
{
struct passwd *getpwuid(), *pw_ptr;
if ( ( pw_ptr = getpwuid( uid ) ) == NULL )
return "Unknown" ;
else
return pw_ptr->pw_name ;
}

char *gid_to_name( gid )
short gid;
/*
* returns pointer to group number gid. used getgrgid(3)
*/
{
struct group *getgrgid(), *grp_ptr;
if ( ( grp_ptr = getgrgid(gid) ) == NULL )
return "Unknown" ;
else
return grp_ptr->gr_name;

}




