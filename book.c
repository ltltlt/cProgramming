#include"book.h"
char already[]="already read";          //it can work because it's not conversion, it's just copy
//char* already="already read";      c++11 not allow conversion from string literal to char*
dirc read={already,NULL,0};
dircptr hashtable[hashNumber];
size_t hash(const char* str){
	int j=0;
	long i=0;
	int len=strlen(str);
	while(j<len){
		i<<=1;
		i^=str[j++];
	}
	i%=hashNumber;
	return (size_t)i;
}
void readFile(void){
	FILE* fp=fopen("/home/ty-l/cProgramming/book/backup7.0/mybook","r");
	if(fp==NULL)
		return;
	char dir[100],bookname[200],*authorname=NULL,*comment=NULL;
	time_t t;
	fgets(dir,100,fp);
	dir[strlen(dir)-1]='\0';
	assert(!strncmp(dir,read.name,strlen(dir)));
	while(fgets(bookname,200,fp)&&*bookname!='\n'){
		bookname[strlen(bookname)-1]='\0';
		authorname=strchr(bookname,',')+1;
		*(authorname-1)='\0';
		comment=strchr(authorname,'@');
		if(comment){
			*comment='\0';
			comment++;
		}
		fscanf(fp,"%ld",&t);
		fgetc(fp);
		add(&read,bookname,authorname,comment,t);
	}
	while(fgets(dir,100,fp)){       //fgets and fputs work that way, if there is a \n in, it can appear, or it won't be add automatical
		dir[strlen(dir)-1]='\0';
		dircptr newdir=(dircptr)malloc(sizeof(dirc));
		assert(newdir);
		newdir->allbook=NULL;
		newdir->number=0;
		newdir->name=(char*)malloc(strlen(dir)+1);
		assert(newdir->name);
		strncpy(newdir->name,dir,strlen(dir));
		size_t i=hash(dir);
		newdir->next=hashtable[i];
		hashtable[i]=newdir;
		while(fgets(bookname,200,fp)&&*bookname!='\n'){
			bookname[strlen(bookname)-1]='\0';
			authorname=strchr(bookname,',')+1;
			*(authorname-1)='\0';
			comment=strchr(authorname,'@');
			if(comment){
				*comment='\0';
				comment++;
			}
			fscanf(fp,"%ld",&t);
			fgetc(fp);
			add(newdir,bookname,authorname,comment,t);
		}
	}
	fclose(fp);
}
int changeAuthor(char* dirname,char* bookname,char* changeTo){
	assert(dirname&&bookname&&changeTo);
	dircptr dir=searchDir(dirname);
	if(!dir)
		return -1;       //no dir
	listptr list=dir->allbook;
	book* b;
	book* newb;
	while(list){
		b=(book*)list->x;
		if(!strcmp(bookname,b->name)){
			newb=(book*)malloc(sizeof(book));
			assert(newb);
			newb->name=b->name;
			newb->author=(char*)malloc(strlen(changeTo)+1);
			strcpy(newb->author,changeTo);
			newb->comment=b->comment;
			newb->t=b->t;
			list->x=(void*)newb;
			free(b->author);
			free(b);
			return 0;
		}
		list=list->next;
	}
	return 1;         //no this book
}
int changeComment(char* dirname,char* bookname,char* changeTo){
	assert(dirname&&bookname&&changeTo);
	dircptr dir=searchDir(dirname);
	if(!dir)
		return -1;            //no dir
	listptr list=dir->allbook;
	book *b,*newb;
	while(list){
		b=(book*)list->x;
		if(!strcmp(bookname,b->name)){
			newb=(book*)malloc(sizeof(book));
			assert(newb);
			newb->name=b->name;
			newb->author=b->author;
			newb->comment=(char*)malloc(strlen(changeTo)+1);
			strcpy(newb->comment,changeTo);
			newb->t=b->t;
			list->x=(void*)newb;
			if(b->comment)
				free(b->comment);
			free(b);
			return 0;
		}
		list=list->next;
	}
	return 1;          //no this book
}
dircptr searchDir(char* dirname){   //get the dir
	assert(dirname);
	dircptr p=hashtable[hash(dirname)];
	while(p){
		if(!strncmp(dirname,p->name,strlen(dirname)))
			return p;
		p=p->next;
	}
	if(!strcmp(dirname,read.name))
		return &read;
	return NULL;
}
int addDir(char* dirname){
	if(searchDir(dirname))
		return 0;
	dircptr newdir=(dircptr)malloc(sizeof(dirc));
	newdir->number=0;
	size_t i=hash(dirname);
	newdir->next=hashtable[i];
	hashtable[i]=newdir;
	newdir->allbook=NULL;
	int len=strlen(dirname);
	newdir->name=(char*)malloc(len+1);
	strncpy(newdir->name,dirname,len+1);
	return 1;
}
int delDir(char* dirname){
	assert(dirname);
	size_t i=hash(dirname);
	dircptr* dp=&hashtable[i];
	dircptr current;
	int len=strlen(dirname); 
	while(*dp){
		current=*dp;
		if(!strncmp(current->name,dirname,len)){
			*dp=current->next;
			if(current->allbook)
				freelist(&current->allbook);
			free(current->name);
			free(current);
			return 1;           //success
		}
		dp=&(*dp)->next;
	}
	return 0;          //false
}
int addBook(char* dirname,char* bookname,char* authorname,char* comment,time_t t){
	dircptr dp=searchDir(dirname);
	if(dp)
		return add(dp,bookname,authorname,comment,t);
	return 0;       //no this dir
}
int delBook(char* dirname,char* bookname){
	dircptr dp=searchDir(dirname);
	if(dp)
		return del(dp,bookname);
	return -1;
}
int add(dircptr d,char* bookname,char* authorname,char* comment,time_t t){
	assert(d&&bookname&&authorname&&t);
	int lenbook=strlen(bookname);
	int lenauth=strlen(authorname);
	book* newbook=(book*)malloc(sizeof(book));
	assert(newbook);
	newbook->name=(char*)malloc(lenbook+1);
	strncpy(newbook->name,bookname,lenbook+1);
	newbook->author=(char*)malloc(lenauth+1);
	strncpy(newbook->author,authorname,lenauth+1);
	newbook->t=t;                  //get time
	if(comment){
		int lencom=strlen(comment);
		newbook->comment=(char*)malloc(lencom+1);
		strncpy(newbook->comment,comment,lencom+1);
	}else
		newbook->comment=NULL;
	if(d->allbook)
		d->allbook=push(d->allbook,newbook);
	else
		d->allbook=createList(newbook);
	++d->number;
	return 1;
}
int del(dircptr d,char* bookname){
	assert(d&&bookname);
	if(d->allbook==NULL)
		return 0;
	listptr* l=&d->allbook;
	book* x;
	while(*l){
		x=(book*)((*l)->x);
		if(!strcmp(x->name,bookname)){
			listptr p=(*l)->next;
			if(read.allbook)
				read.allbook=push(read.allbook,(*l)->x);
			else
				read.allbook=createList((*l)->x);
			*l=p;
			d->number--;
			read.number++;
			return 1;        //success
		}
		l=&(*l)->next;
	}
	return 0;       //Not found
}
int delInRead(char* bookname){
	assert(bookname);
	listptr *l=&read.allbook;
	book* x;
	listptr current;
	while(*l){
		current=*l;
		x=(book*)current->x;
		if(!strcmp(bookname,x->name)){
			*l=current->next;
			--read.number;
			free(x->name);free(x->author);free(x->comment);
			free(x);
			free(current);
			return 1;          //success
		}
		l=&current->next;
	}
	return 0;                   //failure
}
void list(FILE* fp,dircptr d){
	assert(d);
	fprintf(fp,"\" %s \" ...  book number: %d \n",d->name,d->number);
	listptr l=d->allbook;
	struct tm* tm;
	book* x;
	while(l){
		x=(book*)(l->x);
		assert(x);
		fprintf(fp,"<<%s>> ... %s ...",x->name,x->author);
		tm=localtime(&x->t);
		fprintf(fp,"  %s",asctime(tm));
		fprintf(fp,"\"%s\"\n",x->comment);
		l=l->next;
	}
	fprintf(fp,"\n");
}
int showOne(char* dirname){        //show one direction
	assert(dirname);
	dircptr dp=searchDir(dirname);
	if(dp){
		list(stdout,dp);
		return 1;               //find it
	}
	return 0;                 //not
}
void show(FILE* fp,int number){		//show some or showall
	dircptr dp;
	int i;
	for(i=0;i<hashNumber;++i){
		dp=hashtable[i];
		while(dp&&dp->number>number){
			list(fp,dp);
			dp=dp->next;
		}
	}
	if(number<=0)		//show read
		showRead(fp);
}
void showRead(FILE* fp){
	list(fp,&read);
}
void showDir(void){
	int num=1;
	int i;
	dircptr dp;
	for(i=0;i<hashNumber;i++){
		dp=hashtable[i];
		while(dp){
			printf("(%d): ",num++);
			printf("\" %s \" ...  book number: %d \n",dp->name,dp->number);
			dp=dp->next;
		}
	}
}
void toFile(void){
	dircptr d;
	listptr l1,l2;
	book* b;
	FILE* fp=fopen("/home/ty-l/cProgramming/book/backup7.0/mybook","w");
	l1=read.allbook;
	fprintf(fp,"%s\n",read.name);
	while(l1){
		l2=l1->next;
		b=(book*)l1->x;
		fprintf(fp,"%s,%s",b->name,b->author);
		if(b->comment)
			fprintf(fp,"@%s",b->comment);
		fprintf(fp,"\n%ld\n",b->t);
		free(b->name);free(b->author);free(b->comment);
		free(b);
		free(l1);
		l1=l2;
	}
	fprintf(fp,"\n");
	read.allbook=NULL;
	int i;
	for(i=0;i<hashNumber;++i){
		d=hashtable[i];
		while(d){
			fprintf(fp,"%s\n",d->name);
			l1=d->allbook;
			while(l1){
				l2=l1->next;
				b=(book*)l1->x;
				fprintf(fp,"%s,%s",b->name,b->author);
				if(b->comment)
					fprintf(fp,"@%s",b->comment);
				fprintf(fp,"\n%ld\n",b->t);
				free(b->name);free(b->author);free(b->comment);
				free(b);
				free(l1);
				l1=l2;
			}
			free(d->name); //changed
			free(d);
			fprintf(fp,"\n");
			d=d->next;
		}
		hashtable[i]=NULL;
	}
	fclose(fp);
}
