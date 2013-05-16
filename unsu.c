// This file is called unsu.c cause it's only the first approach and it's unsecure!

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include<sys/types.h>

#define MAX_PW_LEN 16

int getpwc(void);
int is_equal(char *str1,char *str2);
int main(int argc,char **argv)
{
	int c=0;
	char s;
	char password[MAX_PW_LEN+1];
	char given_pwd[MAX_PW_LEN+1];
	FILE *f;

	/*I've to setuid here for open files in /ststem directory!*/
	setuid(0);

	//check if shadow file exists
	if((f=fopen("/system/etc/shadow","r"))==NULL)
	{
		//if it doesn't let's create it
		if((f=fopen("/system/etc/shadow","w+"))!=NULL)
		{
			//TODO: insert and crypt root password
			printf("Insert new root password:");
			while(((s=getpwc())!='\n')&&(c<(MAX_PW_LEN+1)))
			{
				printf("*");
				given_pwd[c++]=s;
			}
			for(;c<=(MAX_PW_LEN+1);c++)
				given_pwd[c]='\0';
			fprintf(f,"%s",given_pwd);

		}
		else
		{
			printf("%s: fatal error\n",argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		//get the correct root password
		while((c<MAX_PW_LEN)&&(s=fgetc(f)))
		{
			password[c]=s;
			c++;
		}
		for(;c<=(MAX_PW_LEN+1);c++)
			password[c]='\0';

		//ask user for password
		c=0;
		printf("Give me the f*****g root password:");
		while(((s=getpwc())!='\n')&&(c<(MAX_PW_LEN+1)))
		{
			printf("*");
			given_pwd[c++]=s;
		}
		for(;c<=(MAX_PW_LEN+1);c++)
			given_pwd[c]='\0';

		printf("\n%s : %s\n",given_pwd,password);
		c=strcmp(password,given_pwd);
		printf("strcmp:%d\n",c);
		c=is_equal(password,given_pwd);
		printf("is_equal:%d\n",c);


		//TODO: crypt given password and check whith the encrtypted root password from shadow
		if(is_equal(password,given_pwd)==0)
		{
			system("/system/bin/sh");
		}
		else
		{
			printf("\nNice try...\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

int getpwc(void)
{
	struct termios oldattr, newattr;
	int ch;

	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
	newattr.c_lflag &= ~( ICANON | ECHO);//knock down keybuffer
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
//	system("stty -echo");//shell out to kill echo
	ch = getchar();
//	system("stty echo");
	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
	return ch;
}

int is_equal(char *str1,char *str2)
{
	int i,l1,l2;
	char c;

	l1=strlen(str1);
	l2=strlen(str2);
	i=0;
	do{
		c=str1[i];
		if(str2[i]!=c)
			return -1;
		i++;
	}while((i<l1)&&(i<l2));

	return 0;
}
