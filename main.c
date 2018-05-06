#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "keyword.h"
#include "scan.c"
#include "parse.h"
#include "symtab.h"
#include "parsee1.c"
#include "codeeg1.c"

void disp(struct treenode *);

int main(int argc,char *argv[])
 {
	enum tokentype tt;
	char buf[24];
	struct treenode * prg,* prev;
	if(argc!=2)
	 {
		printf("illegal parameters\n");
		return 1;
	 }
	f=fopen(argv[1],"r");
	if(!f)
	 {
		printf("file does not exist\n");
		return 1;
	 }		
	prg=declaration_list();
	if(mainflag)	
	 {
		//disp(prg);
		generate(prg);
	 }
	else
		printf("\nerror : main function absent or misplaced\n");	
	fclose(f);
	return 0;
 }

void disp(struct treenode *prg)
 {
	struct arg *aa=NULL;
	while(prg)
	 {
		if(prg->nodek==deck  && (prg->kind.dec==vardec || prg->kind.dec==fundec || prg->kind.dec==arrdec))
		 {
			//printf("id : %s\n",prg->attr.name);
			if(prg->kind.dec==fundec)
			 {
				printf("function name : %s\n",prg->attr.fun.name);
				printf("return type : ");
				prg->attr.fun.rettype?printf("int"):printf("void");
				printf("\nparameters...");
				aa=prg->attr.fun.list;
				while(aa)
				 {
					printf(" %s ",aa->info);
					aa=aa->next;
				 }
				printf("\n");
			 }
			else if(prg->kind.dec==arrdec)
			 {
				printf("array name : %s\tsize : %d\n",prg->attr.arr.name,prg->attr.arr.size);
			 }
			else
				printf("id : %s\n",prg->attr.name);
		 }
		else if(prg->nodek==expk)
		 {
	//		if(prg->kind.exp==idk)
	//			printf("%s\n",prg->attr.name);
			if(prg->kind.exp==opk)
				printf("%d\n",prg->attr.op);
			else if(prg->kind.exp==digk)
				printf("%d\n",prg->attr.val);
			else if(prg->kind.exp==funk)
				printf("%s\n",prg->attr.fun.name);
			else
				printf("%s\n",prg->attr.name);
		 }
		if(prg->child[0])
		 {
			disp(prg->child[0]);
		 }
		if(prg->child[1])
		 {
			disp(prg->child[1]);
		 }
		if(prg->child[2])
		 {
			disp(prg->child[2]);
		 }
//		prev=prg;
		if(prg)		
			prg=prg->sibling;
//		free(prev);
	 }	
 }	
		
	
