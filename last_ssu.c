/*
***  secsysu 
***  v. beta
***  
***  Copyright 2013, rock (@_MrRock_)
***  Copyright 2013, tryin (@ax_tryin)
***  
***
***  Licensed under the Apache License, Version 2.0 (the "License");
***  you may not use this file except in compliance with the License.
***  You may obtain a copy of the License at
***
***     http://www.apache.org/licenses/LICENSE-2.0
***
***  Unless required by applicable law or agreed to in writing, software
***  distributed under the License is distributed on an "AS IS" BASIS,
***  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
***  See the License for the specific language governing permissions and
***  limitations under the License.
***
***
**/

//TODO: check if libraries are all necessary
//TODO: clean mem
//TODO: stay setuidded only for the necessary instruction
#include<stdio.h>
#include<string.h>
#include<sha.h>
#include<rand.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

#define MAX_PW_LEN 16
#define SALT_LEN 8		

int getpwc(void);
unsigned char *spc_rand(unsigned char *buf, size_t l);

int main(int argc,char **argv)
{
	int c=0;
	char s;
	unsigned char password[SHA512_DIGEST_LENGTH*2+1];
	unsigned char digest[SHA512_DIGEST_LENGTH];
	char given_pwd[MAX_PW_LEN+SALT_LEN+1];	
	char mdString[SHA512_DIGEST_LENGTH*2+1];
	char res[SHA512_DIGEST_LENGTH*2+SALT_LEN+1+1];
	FILE *f;
	int i;
	unsigned char salt[SALT_LEN];
	

	printf("==============================");
	printf("\n SECsu...\n   v. beta\n");
	printf("==============================\n");

	int myUID = getuid();
	printf("myUID1:%d\n",myUID);
	setuid(0);
	printf("userid0: %d\n",getuid());
	setuid(myUID);
	printf("myUID2:%d\n",myUID);
	printf("userid non zero: %d\n",getuid());
	setuid(0);
	printf("userid0: %d\n",getuid());
	
	//check if shady exist
	if((f=fopen("/system/xbin/shady","r"))==NULL)
	{

		//if it doesn't let's create it
		if((f=fopen("/system/xbin/shady","w+"))!=NULL)
		{
			//setuid(-1);
			printf("> Welcome, no shady file is found!\n");
			printf("> This seems to be the first time you're using ssu..\n");
			printf("> Insert new root password:");
			while(((s=getpwc())!='\n')&&(c<(MAX_PW_LEN+1)))
			{
				printf("*");
				given_pwd[c++]=s;
			}
			//printf("New password in plaintext:\n>%s<",given_pwd);
			/*-------------------------------------------
			 * get the salt and crypt all 
		 	* -------------------------------------------*/		
			/******************************************/
			spc_rand((unsigned char*)&salt,SALT_LEN);
			//printf("\nsalt: >%s<\n",salt);
			for(i=0;i<SALT_LEN;i++)
			{
				given_pwd[c]=salt[i];
				c++;
			}
			given_pwd[c] = '\0';//TODO:clean
			//printf("password_with_salt:\n>%s<",given_pwd);
			/******************************************/						
			SHA512((unsigned char*)&given_pwd, strlen(given_pwd), (unsigned char*)&digest);
			for(i = 0; i < SHA512_DIGEST_LENGTH; i++)
				sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
			//printf("SHA512 digest:>%s<\n", mdString);

			c = 0;
			int l;
			l=strlen(mdString);	
			for(i=0;i<l;i++)
			{
				res[c]=mdString[i];
				c++;
			}
			res[c]='@';
			c++;
			res[c]='\0';
			for(i=0;i<SALT_LEN;i++)
			{
				res[c]=salt[i];
				c++;
			}
			res[c]='\0';
			//printf("\nres:---> %s\n",res);

			fprintf(f,"%s",res);
			//printf(">>>strlen(mdString):%d\n", strlen(mdString));
			 printf("\nDone, call ssu again..\n");
			// system("/system/bin/sh");
		}
		else
		{
			printf("%s: fatal error\n",argv[0]);
			exit(EXIT_FAILURE);
		}        
	}
	else
	{
		/*-------------------------------------------
		 * get correct root password and salt from shady  
		 * -------------------------------------------*/		
		c=0;
						//e se trova una @ nel digest?
		while(c<((SHA512_DIGEST_LENGTH*2))&&((s=fgetc(f))!='@'))
		{
			password[c]=s;
			c++;
		}
		password[c]='\0'; 
		//printf("\nPASSW >%s<",password);
		fgetc(f);
		for(i=0;i<SALT_LEN;i++)
		{
			s=fgetc(f);
			salt[i]=s;
		}
		//printf("\nSALT >%s<",salt);
       		//printf("\npassword readed in shady:>%s<", password);
		//printf("\nstrlen(password)=%d",strlen(password));

		/*-------------------------------------------
		  ask user for password  
		-------------------------------------------*/
		c=0;
		printf("\nType root password:");
		while(((s=getpwc())!='\n')&&(c<(MAX_PW_LEN+1)))
		{
			printf("*");
			given_pwd[c]=s;
			c++;
		}
		for(i=0;i<SALT_LEN;i++)
		{
			given_pwd[c]=salt[i];
			c++;
		}
		given_pwd[c] = '\0';//TODO:clean
	
		//printf("\npassword inserted plain: >%s<",given_pwd);
		SHA512((unsigned char*)&given_pwd, strlen(given_pwd), (unsigned char*)&digest);
		char mdString[SHA512_DIGEST_LENGTH*2+1];
		for(i = 0; i < SHA512_DIGEST_LENGTH; i++)
			sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
		//printf("\ngiven_pwd crypted:>%s<", mdString);
		//printf("\n%d\n",strlen(mdString));
		//printf("\n%d\n",strlen(password));
		//printf(">strcmp: %d\n",strcmp(mdString,password));	
		mdString[SHA512_DIGEST_LENGTH*2+1]='\0';
		//printf("\npassword inserted: >%s<",given_pwd);
		//printf("\nstrcmp: %d\n",strcmp(mdString,password));
		if(strcmp(mdString,password)==0)
		{
			printf("\nWelcome back..\n");
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

//int...? Serve per fare la sporca del non rendere visibile il carattere..
//		  tanto poi si casta a char implicitamente senza troppi problemi =)
int getpwc(void)
{
        struct termios oldattr, newattr;
        int ch;

        tcgetattr( STDIN_FILENO, &oldattr );
        newattr = oldattr;
        newattr.c_lflag &= ~( ICANON | ECHO);//knock down keybuffer
        tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
//      system("stty -echo");//shell out to kill echo
        ch = getchar();
//      system("stty echo");
        tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
        return ch;
}

unsigned char *spc_rand(unsigned char *buf, size_t l) 
{
	if (!RAND_bytes(buf, l)) 
	{
		fprintf(stderr, "PRNG not seeded!\n");
		abort();
	}
	return buf;
}
