#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void insert();
void delete();
void display();
void commit();
void display_versions();
char *itoaa(int , int);
int lines=0,version=0;
int main()
{
	int op,op2;
	char ch;
	printf("welcome to simple version control system..");
	FILE *fp;
	
	fp=fopen("test1.txt","r");
	if(fp==NULL)
	{
		printf("error opening the file..");
		exit(1);
	}
	while( ( ch = fgetc(fp) ) != EOF )
	{
		if(ch=='\n')
		lines++;
	}
	
	do{
	printf("\npresent no of lines are %d",lines);
	printf("\nchoose your option");
	printf("\n------MENU-----");
	printf("\n 1)insert\n2)delete\n3)display\n4)commit");
	scanf("%d",&op);
	switch(op)
	{
	 case 1: 
	 		insert();
			break;
	 case 2: delete();
			break;
	case 3: display();
			break;
	case 4:commit();
			break;
	default:
			printf("incorrect choice");
	}
	printf("\ndo you want to continue?");
	printf("\n1. yes \n 2. no");
	scanf("%d",&op2);
	}while(op2 != 2);
	
	return 0;
}

void insert()
{
	if(lines<=20)
	{
		FILE *fp;
		int ch_no=0;
		char newline[1000],c;
		fp=fopen("test1.txt","a");
		if(fp==NULL)
		{
			printf("error opening the file..");
			exit(1);
		}
		printf("please enter the line you want to enter into the file");
		
		printf("\nEnter a sentence:\n");
    	scanf("%s",newline);
    	ch_no=strlen(newline);
    	if(ch_no<=10)
    	{
    		fprintf(fp,"%s",newline);
    		lines++;
    		fclose(fp);
    		printf("modified lines are as follows..");
			fp = fopen("test1.txt", "r");
        	c = getc(fp);
        	while (c != EOF) 
        	{
            	printf("%c", c);
            	c = getc(fp);
        	}
        	fclose(fp);
        }
        else
        printf("please enter characters less than 9");
       
    }
    else
    printf("the number of lines are getting more than limit(20)");
}

void delete()
{
		 
        FILE *fp1, *fp2;
        char c;
        int del_line, temp = 1;
        fp1 = fopen("test1.txt", "r");
        c = getc(fp1);
        while (c != EOF)
        {
          printf("%c", c);
          c = getc(fp1);
        }
        rewind(fp1);
        printf(" \n Enter the line number of the line to be deleted:");
        scanf("%d", &del_line);
        fp2 = fopen("copy.c", "a");
        c = getc(fp1);
        putc(c, fp2);
        while (c != EOF) 
        {
          c = getc(fp1);
          if (c == '\n')
          temp++;
          
          if (temp != del_line)
          {
            //copy all lines in file copy.c except the line to be deleted
            putc(c, fp2);
          }
        }
        fclose(fp1);
        fclose(fp2);
        //remove original file and rename the modified file as same as the original file name
        remove("test1.txt");
        rename("copy.c", "test1.txt");
        fp1=fopen("test1.txt","a");
        fprintf(fp1,"%c",'\n');
        fclose(fp1);
        printf("\n The contents of file after being modified are as  follows:\n");
        fp1 = fopen("test1.txt", "r");
        c = getc(fp1);
        while (c != EOF) {
            printf("%c", c);
            c = getc(fp1);
        }
        fclose(fp1);
        lines--;
}

void display()
{
	FILE *fp;
	char ch;
	fp=fopen("test1.txt","r");
	if(fp==NULL)
	{
		printf("error opening the file..");
		exit(1);
	}
	
   	while( ( ch = fgetc(fp) ) != EOF )
      printf("%c",ch);
 
  	fclose(fp);
}

void commit()
{
	printf("\nyou have chosen to commit a present test1 file as a new version..\n");
	version++;
	 
	FILE *fp1, *fp2;
    char c;
    int ch;
    
    char name[20];
   
    fp1 = fopen("test1.txt", "r");
    strcpy(name,"version");
    strcat(name,itoaa(version,10));
   	fp2 = fopen(name, "w");
    while ((c = getc(fp1))!= EOF) 
    {
       putc(c, fp2);
    }
    fclose(fp1);
    fclose(fp2);
	printf("do you want to display a particular version? (press 1 if yes)");
	scanf("%d",&ch);
	if(ch == 1)
		display_versions();
}


void display_versions()
{
	int i;
	FILE *fp;
	char name[20],c;
	for(i=0;i<version;i++)
	{
    	strcpy(name,"version");
    	strcat(name,itoaa(i+1,10));
		fp=fopen(name,"r");
		printf("\n-----version %d------\n",i+1);
		c=fgetc(fp);
    	while (c != EOF) 
    	{
      		 printf("%c",c);
      		 c=fgetc(fp);
    	}
		
	}
}

char *itoaa(int val, int base)
{
	static char buf[32] = {0};

	int i = 30;

	if(val==0)
	{
			buf[i]="0123456789abcdef"[val % base];
			return &buf[i];
	}
	for( ; val && i ; --i, val /= base)
		buf[i]="0123456789abcdef"[val % base];

	return &buf[i+1];
}
//output
/*
shwetali@ubuntu:~/Desktop$ gcc main1.c
shwetali@ubuntu:~/Desktop$ ./a.out
welcome to simple version control system..
present no of lines are 3
choose your option
------MENU-----
 1)insert
2)delete
3)display
4)commit3
hello
world
pict

do you want to continue?
1. yes 
 2. no1

present no of lines are 3
choose your option
------MENU-----
 1)insert
2)delete
3)display
4)commit1
please enter the line you want to enter into the file
Enter a sentence:
pune
modified lines are as follows..hello
world
pict
pune
do you want to continue?
1. yes 
 2. no1

present no of lines are 4
choose your option
------MENU-----
 1)insert
2)delete
3)display
4)commit4

you have chosen to commit a present test1 file as a new version..
do you want to display a particular version? (press 1 if yes)1

-----version 1------
hello
world
pict
pune
do you want to continue?
1. yes 
 2. no1

present no of lines are 4
choose your option
------MENU-----
 1)insert
2)delete
3)display
4)commit3
hello
world
pict
pune
do you want to continue?
1. yes 
 2. no1

present no of lines are 4
choose your option
------MENU-----
 1)insert
2)delete
3)display
4)commit2
hello
world
pict
pune 
 Enter the line number of the line to be deleted:3

 The contents of file after being modified are as  follows:
hello
world
pune
do you want to continue?
1. yes 
 2. no1

present no of lines are 3
choose your option
------MENU-----
 1)insert
2)delete
3)display
4)commit4

you have chosen to commit a present test1 file as a new version..
do you want to display a particular version? (press 1 if yes)1

-----version 1------
hello
world
pict
pune
-----version 2------
hello
world
pune
do you want to continue?
1. yes 
 2. no2
shwetali@ubuntu:~/Desktop$ 

*/
