#include"book.h"
#include<unistd.h>      //to use sleep function
#define getChar(c) c=getchar(); \
					getchar()
void userface(void);
int main(void){
	readFile();
	userface();
    toFile();
	return 0;
}
void userface(void){
	printf("Welcome to ty-l's book management system.\n");
	char c;
	char buffer[100],dir[100],file[100],author[100],comment[100];
	char choose[10];
	int flag;
	FILE* fp;
	while(1){
		sleep(2);
		printf("choose: A(add direction)           a(add book)\n");
		printf("        D(delete direction)        d(delete book)\n");
		printf("        S(show direction)          s(show book)\n");
		printf("        C(change comment)          c(change Author)\n");
		printf("        e(delete book in read)     q(quit)\n");
		getChar(c);
		switch(c){
			default:
				if(c-'q')
					printf("Stupid man can never understand clever machine\n");
				else
					printf("Goodbye!\n");
				return;
			case 'c':
				printf("Type the dir: ");
				fgets(dir,100,stdin);
				dir[strlen(dir)-1]='\0';
				printf("Type the book name: ");
				fgets(buffer,100,stdin);
				buffer[strlen(buffer)-1]='\0';
				printf("Who's the author? ");
				fgets(author,100,stdin);
				author[strlen(author)-1]='\0';
				printf("Wait a moment ...\n");
				sleep(1);
				flag=changeAuthor(dir,buffer,author);
				if(0==flag)
					printf("Done!\n");
				else if(flag==1)
					printf("Can't find this book! Check it!.\n");
				else if(flag==-1)
					printf("Can't find this direction! Check it.\n");
				break;
			case 'C':
				printf("Type the dir: ");
				fgets(dir,100,stdin);
				dir[strlen(dir)-1]='\0';
				printf("Type the book name: ");
				fgets(buffer,100,stdin);
				buffer[strlen(buffer)-1]='\0';
				printf("Type comment:\n ");
				fgets(comment,100,stdin);
				comment[strlen(comment)-1]='\0';
				printf("Wait a moment ...\n");
				sleep(1);
				flag=changeComment(dir,buffer,comment);
				if(0==flag)
					printf("Done!\n");
				else if(flag==1)
					printf("Can't find this book! Check it!.\n");
				else if(flag==-1)
					printf("Can't find this direction! Check it.\n");
				break;
			case 'e':
				printf("Type the book name: ");
				fgets(buffer,100,stdin);
				buffer[strlen(buffer)-1]='\0';
				printf("Hint: you delete it, you delete forever.\n Delete whatever? (y)es or (n)o\n");
				getChar(c);
				if(c=='n')
					break;
				printf("Wait a moment ...\n");
				sleep(1);
				if(delInRead(buffer))
					printf("Delete successfully!\n");
				else
					printf("You never read this book, do you forget?\n");
				break;
			case 'A':
				printf("Type direction: ");
				fgets(dir,100,stdin);
				dir[strlen(dir)-1]='\0';
				printf("Adding! Wait a second ...\n");
				sleep(1);
				if(addDir(dir))
					printf("Add successfully!\n");
				else
					printf("%s already exists. Please check.\n",dir);
				break;
			case 'D':
				printf("Type direction: ");
				fgets(dir,100,stdin);
				dir[strlen(dir)-1]='\0';
				printf("Hint: you delete it, and you can never find it and it's books!!!\n");
				printf("Should i show you all books you will delete? (y)es or (n)o: ");
				getChar(c);
				if(c=='y'){
					if(!showOne(dir)){
						printf("Can't find this direction, check it!\n'");
						break;
					}
					else{
						printf("Delete it whatever? (y)es or (n)o: ");
						getChar(c);
						if(c=='n')
							break;
					}
				}
				printf("Deleting! Wait a moment ...\n");
				sleep(1);
				if(delDir(dir))
					printf("Delete successfully!\n");
				else
					printf("Can't find this direction, check it!\n");
				break;
			case 'd':
				printf("Type book name: ");
				fgets(buffer,100,stdin);
				buffer[strlen(buffer)-1]='\0';
				printf("Type direction: ");
				fgets(dir,100,stdin);
                dir[strlen(dir)-1]='\0';
				printf("Deleting! Wait a moment ...\n");
				sleep(1);
				flag=delBook(dir,buffer);
				if(1==flag)
					printf("Delete successfully, find it in alread read.\n");
				else if(-1==flag)
					printf("Can't find this direction, check it.\n");
				else
					printf("Can't find this book in %s, check it.\n",dir);
				break;
			case 's':
				printf("Type:\n");
				printf("     some(show some interesting books)\n");
				printf("     all (show all interesting books)\n");
				printf("     read(show the book you already read)\n");
				printf("     one (show some book in one specific direction)\n");
				scanf("%s",choose);
				getchar();
				if(!strncmp(choose,"some",4)){
					printf("Working! Wait a moment ...\n");
					sleep(1);
					show(stdout,3);
				}
				else if(!strncmp(choose,"all",3)){
					printf("Working! Wait a moment ...\n");
					sleep(1);					
					show(stdout,-1);
					sleep(2);
					printf("Want to see it in file? (y)es or (n)o\n");
					getChar(c);
					if(c=='y'){
						printf("Type file name: ");
						fgets(file,100,stdin);
						file[strlen(file)-1]='\0';
						fp=fopen(file,"w");
						printf("Wait a moment  ...\n");
						show(fp,-1);
						fclose(fp);
						sleep(1);
						printf("Done!\n");
					}
				}else if(!strncmp(choose,"read",4)){
					printf("Working! Wait a moment ...\n");
					sleep(1);
					showRead(stdout);
				}else if(!strncmp(choose,"one",3)){
					printf("Type direction: ");
					fgets(dir,100,stdin);
					dir[strlen(dir)-1]='\0';
					printf("Working! Wait a moment ...\n");
					sleep(1);
					if(!showOne(dir))
						printf("Can't find this direction, check it.\n");
				}
				break;
			case 'S':
				printf("Wait, i will show you all directions\n");
				sleep(1);
				showDir();
				break;
			case 'a':
				printf("Type book name: ");
				fgets(buffer,100,stdin);
				buffer[strlen(buffer)-1]='\0';
				printf("Type author: ");
				fgets(author,100,stdin);
				author[strlen(author)-1]='\0';
				printf("Type direction: ");
				fgets(dir,100,stdin);
				dir[strlen(dir)-1]='\0';
				printf("Type comment:\n");
				fgets(comment,100,stdin);
				printf("Adding! Wait a moment ...\n");
				sleep(1);
				dircptr p=hashtable[hash(dir)];
				listptr l;
				book* b;
				flag=1;
				char temp1[100],temp2[100];
				toLittle(temp2,dir);
				while(p&&flag==1){
					toLittle(temp1,p->name);
					if(!strncmp(temp1,temp2,strlen(dir))){
						flag=2;					//find this dir
						l=p->allbook;
						toLittle(temp1,buffer);
						while(l){
							b=(book*)l->x;
							toLittle(temp2,b->name);
							if(!strncmp(temp1,temp2,strlen(buffer))&&!strcmp(b->author,author)){
								printf("This book already exists,still add it? (y)es or (n)o: ");
								getChar(c);
								if(c=='y')
									flag=0;          //still add
								else
									flag=-1;        //not add
								break;
							}
							l=l->next;
						}
					}
					p=p->next;
				}
				if(flag==-1){             //not add
					break;
				}else if(flag==1)
					printf("Can't find this direction, check it.\n");
				else if(flag!=2&&flag!=0)
					printf("Unknown error!\n");
				time_t t;
				time(&t);
				char* com;
				if('\n'==*comment)
					com=NULL;
				else{
					com=comment;
					comment[strlen(com)-1]='\0';
				}
				if(addBook(dir,buffer,author,com,t))
					printf("Adding successfully!\n");
				else
					printf("Can't find this direction, check it.\n");
				break;
		}
	}
}
