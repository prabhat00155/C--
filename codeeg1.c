char curv[32];
char local[12][32];
int jflag,lv,lvv=1,lvf=1,sflag;

void express(struct treenode *,int,int);
void stmt(struct treenode *,int,int);
int retr(int);

void generate(struct treenode *r)
 {
	int flag=0,flag1=0;
	int val=-1,lindex=-1;
	printf("%s %s\n","include","lib.asm");
	while(r)
	 {
		val=-1;
		lindex=-1;
		if(r->nodek==deck)
		 {
			if(!flag1)
			 	printf("data segment\n");
			while(!flag1 && r)
			 {
				if(r->kind.dec==vardec)
					printf("%s	dw	0\n",r->attr.name);
				else if(r->kind.dec==arrdec)
					printf("%s	dw	%d	dup(0)\n",r->attr.arr.name,r->attr.arr.size);
				else
				 {
					printf("%s	dw	%d	dup(0)\n","locvar",12);
					printf("%s\n","data ends");
					flag1=1;
					printf("\n%s\n","code segment");
					printf("assume cs:code,ds:data\n");
				 }
				if(!flag1)
					r=r->sibling;
			 }
			if(r->kind.dec==fundec)	
			 {
				struct treenode *f;
				int res=0;
				f=r->child[0];
				if(!strcmp(r->attr.fun.name,"main"))
				 {
					printf("%s\n","start:");
					printf("%s %s,%s\n","mov","ax","data");
					printf("%s %s,%s\n","mov","ds","ax");
					flag=1;
				 }
				else
				 {
					struct arg *rhea;
					int i=0;
					printf("%s%d\t%s\n",r->attr.fun.name,r->attr.fun.narg,"proc");	// display function name ending with no. of args
				 	rhea=r->attr.fun.list;
					lindex=r->attr.fun.narg-1;
					while(rhea)
					 {
						strcpy(local[i++],rhea->info);
			//		printf("__ %s  %s\n",local[0],rhea->info);
						rhea=rhea->next;
					//printf("__ %s  %s\n",local[0],rhea->info);
					 }
			//		printf(",, %s\n",local[0]);
					/*for(i=lindex;i>=0;i--)
						printf("%s %s[%d]\n","pop","locvar",i);*/
				 }
				while(f)
				 {
					if(f->nodek==deck)
				 	 {
						if(f->kind.dec==vardec)
					 	 {
							strcpy(local[++lindex],f->attr.name);
						 }
					// 	else if(f->kind.dec==arrdec)			// arrays 	check!
					//		add(t,f->attr.arr.name);
					 }
					else if(f->nodek==stmtk)
					 {
						if(f->kind.stmt==expstmt)		// expression
						 {
							express(f->child[0],lindex,val);		// translate the expression
						 }
						else if(f->kind.stmt==selstmt)
						 {
							express(f->child[0],lindex,val);
							stmt(f->child[1],lindex,val);	// call generate() itself, with flag1=1
							printf("%s %s%d\n","jmp","L",lv+1);	
							printf("%s%d:\n","L",lv++);
							stmt(f->child[2],lindex,val);		
							printf("%s%d:\n","L",lv++);
						 }
						else if(f->kind.stmt==iterstmt)
						 {
							printf("%s%d:\n","L",lv++);
							express(f->child[0],lindex,val);
						 	stmt(f->child[1],lindex,val);
							printf("%s %s%d\n","jmp","L",lv-1);
							printf("%s%d:\n","L",lv++);	
						 }
						else if(f->kind.stmt==retstmt)
						 {
							express(f->child[0],lindex,val);			// return value in bx
							//printf("%s\n","ret");
						 }
 					 }	
					f=f->sibling;								
				 }
				if(!flag)
				 {
					printf("%s\n","ret");
					printf("%s%d\t%s\n",r->attr.fun.name,r->attr.fun.narg,"endp");
				 }
				else
				 {
					printf("%s %s,%s\n","mov","ah","4ch");
					printf("%s %s\n","int","21h");
					printf("%s\n","code ends");
				 }
			 }	
		 }
		r=r->sibling;		
	 }
	printf("%s","end start\n");
 }

void express(struct treenode *t,int lindex,int val)
 {
	int gstatus,lvl;
	if(sflag)
		lvl=lvf*10+lvv;
	else
		lvl=lv;
	if(t->nodek==expk)	
	 {
		if(t->kind.exp==opk)
		 {
		//	struct treenode *t=f;
		//	while(t->kind.exp==opk)			// put this inside a function and call it recursively
			 {
				switch(t->attr.op)
				 {
				 	case EQUAL:
							if(t->child[1])
								express(t->child[1],lindex,val);
							if(t->child[0])
							 {
								if(t->child[0]->kind.exp==idk)
								 {
									strcpy(curv,t->child[0]->attr.name);
									gstatus=retr(lindex);
									if(gstatus!=-1)
										printf("%s %s[%d],%s\n","mov","locvar",gstatus,"bx");	
									else
										printf("%s %s,%s\n","mov",curv,"bx");	
								 }
								else if(t->child[0]->kind.exp==arrk)
								 {
									printf("%s %s,%s\n","mov","dx","bx");
									express(t->child[0]->child[0],lindex,val);
									printf("%s %s[%s],%s\n","mov",t->child[0]->attr.name,"bx","dx");
								 }
							 }
							break;
					case PLUS:
							if(t->child[1]->attr.op!=ASTERISK && t->child[1]->attr.op!=SLASH)
							 {
								express(t->child[0],lindex,val);
								printf("%s %s,%s\n","mov","ax","bx");
								express(t->child[1],lindex,val);
								printf("%s %s,%s\n","add","bx","ax");
							 }
							else
							 {
								express(t->child[1],lindex,val);
								printf("%s %s,%s\n","mov","ax","bx");
								express(t->child[0],lindex,val);
								printf("%s %s,%s\n","add","bx","ax");
							 }
							break;
					case MINUS:	
							if(t->child[1]->attr.op!=ASTERISK && t->child[1]->attr.op!=SLASH)
							 {
								express(t->child[0],lindex,val);
								printf("%s %s,%s\n","mov","ax","bx");
								express(t->child[1],lindex,val);
								printf("%s %s,%s\n","sub","ax","bx");
								printf("%s %s,%s\n","mov","bx","ax");
							 }
							else
							 {
								express(t->child[1],lindex,val);
								printf("%s %s,%s\n","mov","ax","bx");
								express(t->child[0],lindex,val);
								printf("%s %s,%s\n","sub","bx","ax");
							 }
							break;
					case ASTERISK:
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");
							express(t->child[1],lindex,val);
							printf("%s %s\n","mul","bx");
							printf("%s %s,%s\n","mov","bx","ax");
							break;
					case SLASH:
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");
							express(t->child[1],lindex,val);
							printf("%s %s,%s\n","mov","dx","00h");
							printf("%s %s\n","div","bx");
							printf("%s %s,%s\n","mov","bx","ax");
							break;
					case MOD:
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");
							express(t->child[1],lindex,val);
							printf("%s %s,%s\n","mov","dx","00h");
							printf("%s %s\n","div","bx");
							printf("%s %s,%s\n","mov","bx","dx");
							break;
					case LT:	
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");								
							express(t->child[1],lindex,val);
							printf("%s %s,%d\n","mov","ah",0);
							printf("%s %s,%d\n","mov","bh",0);
							printf("%s %s,%s\n","cmp","ax","bx");
							printf("%s %s%d\n","jae","L",lvl);
							//jflag=0;
							break;
					case LE:
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");								
							express(t->child[1],lindex,val);
							printf("%s %s,%d\n","mov","ah",0);
							printf("%s %s,%d\n","mov","bh",0);
							printf("%s %s,%s\n","cmp","ax","bx");
							printf("%s %s%d\n","ja","L",lvl);
							//jflag=1;
							break;
					case GT:
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");								
							express(t->child[1],lindex,val);
							printf("%s %s,%d\n","mov","ah",0);
							printf("%s %s,%d\n","mov","bh",0);
							printf("%s %s,%s\n","cmp","ax","bx");
							printf("%s %s%d\n","jbe","L",lvl);
							//jflag=2;
							break;
					case GE:	
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");								
							express(t->child[1],lindex,val);
							printf("%s %s,%d\n","mov","ah",0);
							printf("%s %s,%d\n","mov","bh",0);
							printf("%s %s,%s\n","cmp","ax","bx");
							printf("%s %s%d\n","jb","L",lvl);
							//jflag=3;
							break;
					case EQ:
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");								
							express(t->child[1],lindex,val);
							printf("%s %s,%d\n","mov","ah",0);
							printf("%s %s,%d\n","mov","bh",0);
							printf("%s %s,%s\n","cmp","ax","bx");
							printf("%s %s%d\n","jne","L",lvl);
							//jflag=4;
							break;
					case NE:
							express(t->child[0],lindex,val);
							printf("%s %s,%s\n","mov","ax","bx");								
							express(t->child[1],lindex,val);
							printf("%s %s,%d\n","mov","ah",0);
							printf("%s %s,%d\n","mov","bh",0);
							printf("%s %s,%s\n","cmp","ax","bx");
							printf("%s %s%d\n","je","L",lvl);
							//jflag=5;
							break;
				 }
			 }
		 }
		else if(t->kind.exp==idk)
		 {
			strcpy(curv,t->attr.name);
			gstatus=retr(lindex);
			if(gstatus!=-1)
				printf("%s %s,%s[%d]\n","mov ","bx","locvar",gstatus);
			else
				printf("%s %s,%s\n","mov","bx",curv);
		 }
		else if(t->kind.exp==digk)
		 {
			printf("%s %s,%d\n","mov","bx",t->attr.val);			
		 }
		else if(t->kind.exp==funk)				// function call
		 {
			if(!strcmp(t->attr.fun.name,"read"))
			 {
				printf("%s\n","get");			// macro	input in bl
				printf("%s %s,%s\n","mov","bl","al");
				printf("%s %s,%d\n","mov","bh",0);
			 }
			else if(!strcmp(t->attr.fun.name,"print"))
			 {
				if(t->child[0])
				 {
					if(t->child[0]->kind.exp==idk)
					 {
						strcpy(curv,t->child[0]->attr.name);
						gstatus=retr(lindex);
					//	printf("...%s...%s...%d\n",curv,local[0],gstatus);
						if(gstatus!=-1)
							printf("%s %s,%s[%d]\n","mov","cx","locvar",gstatus);
						else
							printf("%s %s,%s\n","mov","cx",curv);
						printf("%s %s\n","put","cl");		//	second parameter shall be the arg to output
					 }
					else if(t->child[0]->kind.exp==digk)
					 {
						printf("%s %d\n","put",t->child[0]->attr.val); 
					 }
					else if(t->child[0]->kind.exp==arrk)
					 {
						express(t->child[0]->child[0],lindex,val);
						printf("%s %s,%s[%s]\n","mov","cx",t->child[0]->attr.name,"bx");
						printf("%s %s\n","put","cl"); 
					 }
					else
					 {
						express(t->child[0],lindex,val);
						printf("%s %s\n","put","bl"); 	
					 }
			 	 }
			 }
			else
			 {
				int i;
				struct treenode *temp=t->child[0];
				for(i=0;i<=lindex;i++)
					printf("%s %s[%d]\n","push","locvar",i);
				i=0;
				while(temp)
				 {
					if(temp->kind.exp==idk)
					 {
						strcpy(curv,temp->attr.name);
						gstatus=retr(lindex);
						if(gstatus!=-1)
						 {
							printf("%s %s,%s[%d]\n","mov","dx","locvar",gstatus);
							printf("%s %s[%d],%s\n","mov","locvar",i++,"dx");
						 }
						else
						 {
							printf("%s %s,%s\n","mov","dx",curv);	
							printf("%s %s[%d],%s\n","mov","locvar",i++,"dx");	
						 }
					 }
					else if(temp->kind.exp==digk)
					 {
						printf("%s %s,%d\n","mov","dx",temp->attr.val);
						printf("%s %s[%d],%s\n","mov","locvar",i++,"dx");	
					 }
					else
					 {
						express(temp,lindex,val);
						printf("%s %s[%d],%s\n","mov","locvar",i++,"bx");	
					 }	
					temp=temp->sibling;
				 }
				printf("%s %s%d\n","call",t->attr.fun.name,t->attr.fun.narg);	// manage the parameters before this
				for(i=lindex;i>=0;i--)
					printf("%s %s[%d]\n","pop","locvar",i);
			 }
		 }
		else if(t->kind.exp==arrk)				// god save these!
		 {
			express(t->child[0],lindex,val);
			printf("%s %s,%s[%s]\n","mov","bx",t->attr.name,"bx");
		 }
	}
 }

void stmt(struct treenode *f,int lindex,int val)
 {
	int skip=0;
	while(f)
	 {
		skip=0;
		if(f->nodek==deck)
		 {
			if(f->kind.dec==blkdec)
			 {
				f=f->child[0];
				skip=1;
			 }
		 }
		else if(f->nodek==stmtk)
		 {
			if(f->kind.stmt==expstmt)		// expression
			 {
				express(f->child[0],lindex,val);		// translate the expression
			 }
			else if(f->kind.stmt==selstmt)
			 {
				int t1,t2;
				sflag=1;
				lvf++;
				express(f->child[0],lindex,val);
				t1=lvf;t2=lvv;
				stmt(f->child[1],lindex,val);	// call generate() itself, with flag1=1
				lvf=t1;lvv=t2;
				printf("%s %s%d\n","jmp","L",lvf*10+lvv+1);	
				printf("%s%d:\n","L",lvf*10+lvv++);
				t1=lvf;t2=lvv;
				stmt(f->child[2],lindex,val);		
				lvf=t1;lvv=t2;
				printf("%s%d:\n","L",lvf*10+lvv++);
				sflag=0;
			 }
			else if(f->kind.stmt==iterstmt)
			 {
				int t1,t2;
				sflag=1;
				lvf++;
				printf("%s%d:\n","L",lvf*10+lvv++);
				express(f->child[0],lindex,val);
				t1=lvf;t2=lvv;
				stmt(f->child[1],lindex,val);
				lvf=t1;lvv=t2;
				printf("%s %s%d\n","jmp","L",(lvf)*10+lvv-1);
				printf("%s%d:\n","L",lvf*10+lvv++);
				sflag=0;
			 }
			else if(f->kind.stmt==retstmt)
			 {
				express(f->child[0],lindex,val);			// return value in bx
				printf("%s\n","ret");
			 }
		 }	
		if(!skip)
			f=f->sibling;								
	 }	
 }

int retr(int index)
 {
	int i=0;
	while(strcmp(curv,local[i]) && i<=index)
	 {
		i++;
	 }
	if(i<=index)
		return i;	
	else
		return -1;
 }


