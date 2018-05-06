int flag,flag1,flag2,flag3,dec_flag,var_flag,estatus;		//stmt_flag;

enum tokentype tt,rettype;

struct treenode * declaration_list(void);
struct treenode * declaration(struct symtab *);
struct arg * params(void);
struct arg * param_list(void);
struct arg * param(void);
struct treenode * compound_stmt(struct symtab *,struct symtab *);
struct treenode * local_declarations(struct symtab *);
struct treenode * var_declarations(struct symtab *);
struct treenode * statement_list(struct symtab *,struct symtab *);
struct treenode * statement(int *,struct symtab *,struct symtab *);
struct treenode * expression_stmt(struct symtab *,struct symtab *);
struct treenode * selection_stmt(struct symtab *,struct symtab *);
struct treenode * iteration_stmt(struct symtab *,struct symtab *);
struct treenode * return_stmt(struct symtab *,struct symtab *);
struct treenode * expression(struct symtab *,struct symtab *);
struct treenode * simple_expression(enum tokentype,struct symtab *,struct symtab *);
struct treenode * additive_expression(enum tokentype,struct symtab *,struct symtab *);
struct treenode * term(enum tokentype,struct symtab *,struct symtab *);
struct treenode * factor(enum tokentype,struct symtab *,struct symtab *);
struct treenode * args(int *,struct symtab *,struct symtab *);
void errhandle(int);
//struct treenode * mknode();

struct treenode * declaration_list()
 {
	struct treenode * dl,* f=NULL;
	int count=0;
	struct symtab * stab=(struct symtab *)malloc(sizeof(struct symtab));
	stab->attr.size=-1;
	strcpy(stab->name,"\0");
	dec_flag=0;
	do
	 {
		//printf("%d",count);
		if(!count++)
		 {
			f=dl=declaration(stab);
			if(dl)
				dl->sibling=NULL;
		 }
		else
		 {
			dl->sibling=declaration(stab);		
			dl=dl->sibling;
			if(dl)
				dl->sibling=NULL;	
		 }
	 }while(!dec_flag);
	return f;
 }

struct treenode * declaration(struct symtab *stab)			// var declaration or fun declaration
 {
	struct treenode * dc=NULL;
	char ptoken[32];
 	tt=gettoken();
	if(tt==INT || tt==VOID)
	 {
		int flag=1;
		if(tt==VOID)
			flag=0;
		tt=gettoken();
		if(tt==ID)
		 {
			strcpy(ptoken,curtok);
			tt=gettoken();
			if(flag && tt==OBB) 	//variable
			 {
				tt=gettoken();
				if(tt==DIGIT)
				 {
					int dig=atoi(curtok);
					tt=gettoken();
					if(tt=CBB)
					 {
						tt=gettoken();
						dc=(struct treenode *)malloc(sizeof(struct treenode));
						dc->child[0]=dc->child[1]=dc->child[2]=NULL;
						dc->nodek=deck;
						dc->kind.dec=arrdec;
						dc->attr.arr.name=(char *)malloc(32);
						strcpy(dc->attr.arr.name,ptoken);		//global var curtoken		:: attention!
						dc->attr.arr.size=dig;				// check dig
						if(!lookup(stab,dc))
							insert(stab,dc);
						else
							errhandle(35);
					 }
					else
						errhandle(1);
				 }
				else
					errhandle(2);
					
			 }
			if(flag && tt==SEMI)
			 {
				if(!dc)
				 {
					dc=(struct treenode *)malloc(sizeof(struct treenode));
					dc->child[0]=dc->child[1]=dc->child[2]=NULL;
					dc->nodek=deck;
					dc->kind.dec=vardec;
					dc->attr.name=(char *)malloc(32);
					strcpy(dc->attr.name,ptoken);		//global var curtoken		:: attention!
				 }
				if(!lookup(stab,dc))						// check var redeclaration
					insert(stab,dc);	
				else
					errhandle(33);	
				return dc;
			 }
			else if(tt==OBR)	//function
			 {
				struct symtab *ltab;
				struct arg *first=params();
				if(tt==CBR)		
				 {
					tt=gettoken();
					dc=(struct treenode *)malloc(sizeof(struct treenode));
					dc->child[0]=dc->child[1]=dc->child[2]=NULL;
					dc->nodek=deck;
					dc->kind.dec=fundec;
					dc->attr.fun.name=(char *)malloc(32);
					strcpy(dc->attr.fun.name,ptoken);		//global var curtoken		:: attention!
					if(strcmp("main",ptoken))
						mainflag=0;
					else
						mainflag=1;		//if it matches
					if(!flag)
					 {
						dc->attr.fun.rettype=voidt;
						rettype=VOID;
					 }
					else
					 {
						dc->attr.fun.rettype=intt;
						rettype=INT;
					 }
					dc->attr.fun.list=first;
					dc->attr.fun.narg=count(first);
					if(!lookup(stab,dc))			// check fun redeclaration
						insert(stab,dc);
					else
						errhandle(34);			
					ltab=(struct symtab *)malloc(sizeof(struct symtab));
					insarg(ltab,first);
					dc->child[0]=compound_stmt(stab,ltab);	// global symtab required in case of function call or use of global var
 				 }
				else
					errhandle(3);
			 }
			else
				errhandle(4);
				
		 }
		else
			errhandle(5);
	 }
	else if(tt==ENDOFFILE)
	 {
		dec_flag=1;
	 }
	else
		errhandle(6);
	return dc;
 }

struct arg * params()
 {
	struct arg * first=NULL;
	tt=gettoken();
	if(tt==VOID)
	 {
		tt=gettoken();
		return NULL;
	 }
	else if(tt==INT)
	 {
		first=param_list();
	 }
	else if(tt==CBR)
	 {
		return first;
	 }
	else
		errhandle(25);
 }

struct arg * param_list()
 {
	struct arg * first=NULL,* n;
	int count=0;	
	do
	 {
		if(!count)
		 {
			count=1;
			first=param();
			n=first;
		 }
		else
		 {
			tt=gettoken();
			if(tt==INT)
			 {
				n->next=param();
				n=n->next;
			 }
		 }
	 }while(tt==COMMA);
	if(tt==CBR)
	 {
		//n->next=NULL;
		return first;
	 }
	else
		errhandle(26);
 }

struct arg * param()
 {
	struct arg *f=NULL;
		tt=gettoken();
	        if(tt==ID)
		 {
			f=(struct arg *)malloc(sizeof(struct arg));
			f->info=(char *)malloc(32);
			strcpy(f->info,curtok);						//current token  :: global variable
			f->next=NULL;
			tt=gettoken();
		 	if(tt==OBB)					//representation of arrays 		attention!
			 {
				tt=gettoken();					//array indices have not been considered
				if(tt==CBB)
				 {
					tt=gettoken();
					return f;
				 }
				else
					errhandle(7);
			 }
			return f;
		 }
		else
			errhandle(8);
 }		

struct treenode * compound_stmt(struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * cs,* t;
	if(tt==OCB)
	 {
		tt=gettoken();
		cs=t=local_declarations(ltab);
		if(t)
		 {
			while(t->sibling!=NULL)
				t=t->sibling;
			t->sibling=statement_list(stab,ltab);
		 }
		else
			cs=statement_list(stab,ltab);
		return cs;
	 }
	else
		errhandle(10);
 }

struct treenode * local_declarations(struct symtab *ltab)
 {
	int flag4=0;
	struct treenode * ld,* f=NULL;
	var_flag=0,flag3=0;
	do
 	 {
		if(!flag4++)
		 {
			f=ld=var_declarations(ltab);
			if(ld)
				ld->sibling=NULL;
		 }
		else
		 {
			ld->sibling=var_declarations(ltab);
			ld=ld->sibling;
			if(ld)
				ld->sibling=NULL;
		 }
		if(!var_flag)
			tt=gettoken();
	 }while(!var_flag);
	return f;
 }

struct treenode * var_declarations(struct symtab *ltab)
 {
	struct treenode * vd=NULL; 
	char *ptoken;
	if(tt==INT)
	 {
		tt=gettoken();
		if(tt==ID)
		 {
			ptoken=curtok;
			tt=gettoken();
			if(tt==OBB)
			 {
				tt=gettoken();
				if(tt==DIGIT)
				 {
					int dig=atoi(curtok);
					tt=gettoken();
					if(tt=CBB)
					 {
						tt=gettoken();
						vd=(struct treenode *)malloc(sizeof(struct treenode));
						vd->child[0]=vd->child[1]=vd->child[2]=NULL;	
						vd->nodek=deck;
						vd->kind.dec=arrdec;
						vd->attr.arr.name=(char *)malloc(32);
						strcpy(vd->attr.arr.name,ptoken);
						vd->attr.arr.size=dig;
						if(!lookup(ltab,vd))
							insert(ltab,vd);
						else
							errhandle(35);	
					 }	
					else
						errhandle(11);
				 }
				else
					errhandle(12);
					
			 }
			if(tt==SEMI)
			 {
				if(!vd)
				 {
					vd=(struct treenode *)malloc(sizeof(struct treenode));
					vd->child[0]=vd->child[1]=vd->child[2]=NULL;
					vd->nodek=deck;
					vd->kind.dec=vardec;
					vd->attr.name=(char *)malloc(32);
					strcpy(vd->attr.name,ptoken);
					if(!lookup(ltab,vd))
						insert(ltab,vd);
					else
						errhandle(36);
				 }
				return vd;
			 }
		 }
		else
			errhandle(13);
	 }
	else
		var_flag=1;
	return vd;
 }

struct treenode * statement_list(struct symtab *stab,struct symtab *ltab)
 {
	int flag=0,flag1=0;
	int stmt_flag=0;
	struct treenode * sl,* f=NULL;
//	print(stab);
	do
 	 {
		estatus=0;
		if(!flag1++)
		 {
			f=sl=statement(&stmt_flag,stab,ltab);
		 }
		else
		 {
			sl->sibling=statement(&stmt_flag,stab,ltab);
			sl=sl->sibling;
		 }
		if(!stmt_flag)
		 {
			if(sl)
				sl->sibling=NULL;
			if(!estatus)
				tt=gettoken();
		 }
		//	tt=gettoken();
	 }while(!stmt_flag);
	return f;
 }

struct treenode * statement(int *stmt_flag,struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * f=NULL;
//	print(stab);
	if(tt==ID || tt== DIGIT || tt==SEMI)
	 {
		f=(struct treenode *)malloc(sizeof(struct treenode));
		f->child[0]=f->child[1]=f->child[2]=NULL;
		f->nodek=stmtk;
		f->kind.stmt=expstmt;
		f->child[0]=expression_stmt(stab,ltab);
	 }
	else if(tt==OCB)
	 {
		f=(struct treenode *)malloc(sizeof(struct treenode));
		f->child[0]=f->child[1]=f->child[2]=NULL;
		f->nodek=deck;
		f->kind.dec=blkdec;
		f->child[0]=compound_stmt(stab,ltab);
	 }
	else if(tt==IF)
		f=selection_stmt(stab,ltab);
	else if(tt==WHILE)
		f=iteration_stmt(stab,ltab);
	else if(tt==RETURN)
		f=return_stmt(stab,ltab);
	else if(tt==CCB)
	 {
		if(stmt_flag)
			*stmt_flag=1;
	 }
	else
		errhandle(14);
	return f;
 }
	
struct treenode * expression_stmt(struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * es=NULL;
//	print(stab);
//	print(ltab);
	if(tt==ID || tt==DIGIT)
		es=expression(stab,ltab);
	if(tt==SEMI)
		return es;
	else
		errhandle(15);
 }

struct treenode * selection_stmt(struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * ss=(struct treenode *)malloc(sizeof(struct treenode));
	//print(stab);
	ss->child[0]=ss->child[1]=ss->child[2]=NULL;
	ss->nodek=stmtk;
	ss->kind.stmt=selstmt;
	tt=gettoken();
	if(tt==OBR)
	 {
		tt=gettoken();
	 	ss->child[0]=simple_expression(tt,stab,ltab);
		//tt=gettoken();
		if(tt==CBR)
		 {
			tt=gettoken();
			ss->child[1]=statement(NULL,stab,ltab);
			tt=gettoken();
			estatus=1;
			if(tt==ELSE)
			 {
				tt=gettoken();
				estatus=0;
				ss->child[2]=statement(NULL,stab,ltab);
			 }
		 }
		else
			errhandle(16);	
	 }
	else
		errhandle(17);
	return ss;	
 }

struct treenode * iteration_stmt(struct symtab *stab,struct symtab *ltab)
 {
	struct treenode *is=(struct treenode *)malloc(sizeof(struct treenode));
	is->child[0]=is->child[1]=is->child[2]=NULL;
	is->nodek=stmtk;
	is->kind.stmt=iterstmt;
	tt=gettoken();
	if(tt==OBR)
	 {
		tt=gettoken();
		is->child[0]=simple_expression(tt,stab,ltab);
		//tt=gettoken();	
		if(tt==CBR)
		 {
			tt=gettoken();
			is->child[1]=statement(NULL,stab,ltab);
		 }
		else
			errhandle(18);
	 }
	else
		errhandle(19);
	return is;
 }

struct treenode * return_stmt(struct symtab *stab,struct symtab *ltab)
 {
	int taken=0;
	struct treenode * rs=(struct treenode *)malloc(sizeof(struct treenode));
	rs->child[0]=rs->child[1]=rs->child[2]=NULL;
	rs->nodek=stmtk;
	rs->kind.stmt=retstmt;
	tt=gettoken();
	if(tt==ID || tt==DIGIT)
	 {
		taken=1;
		if(rettype==INT)
			rs->child[0]=simple_expression(tt,stab,ltab);
		else
			errhandle(30);
	 }
 	if(tt==SEMI)
	 {
		if(!taken && rettype==INT)
			errhandle(31);
		return rs;
	 }
	else
		errhandle(20);
 }

struct treenode * expression(struct symtab *stab,struct symtab *ltab)
 {
	enum tokentype pt;
	char ptok[32];
	struct treenode * exp=(struct treenode *)malloc(sizeof(struct treenode));
	exp->child[0]=exp->child[1]=exp->child[2]=NULL;
//	print(stab);
//	print(ltab);
	if(tt==ID)
	 {
		struct treenode * oper=(struct treenode *)malloc(sizeof(struct treenode));
		oper->child[0]=oper->child[1]=oper->child[2]=NULL;
		oper->nodek=expk;
		strcpy(ptok,curtok);
		//oper->kind.exp=idk;
		//oper->child[0]=oper->child[1]=NULL;
		//exp->child[0]=oper;	
	//	printf("%d--%s--\n",exp->child[0]->kind.exp,exp->child[0]->attr.name);
		pt=tt;
		tt=gettoken();
		//exp=(struct treenode *)malloc(sizeof(struct treenode));
		if(tt==OBB)							//array
		 {
			tt=gettoken();
			oper->child[0]=additive_expression(pt,stab,ltab);
		 	if(tt==CBB)
			 {
				oper->attr.name=(char *)malloc(32);
				strcpy(oper->attr.name,ptok);
				oper->kind.exp=arrk;
				pt=tt;
				tt=gettoken();
			 }
			else
				errhandle(21);
		 }
		else if(tt==OBR)
		 {
		//	fc->nodek=expk;
			int tmp=0;
			oper->attr.fun.name=(char *)malloc(32);
			strcpy(oper->attr.fun.name,ptok);			//global var curtok
			tt=gettoken();
			if(tt!=CBR)
			 {	
				oper->child[0]=args(&tmp,stab,ltab);
				oper->attr.fun.narg=tmp;
			 }
			if(tt==CBR)
			 {
				oper->kind.exp=funk;
				if(!lib(oper) && !lookup(stab,oper))			// check fun definition
					errhandle(46);
				tt=gettoken();
				return oper;
			 }
			else
				errhandle(24);
		 }
		else
		 {
			oper->attr.name=(char *)malloc(32);
			strcpy(oper->attr.name,ptok);		
			oper->kind.exp=idk;	
   		 }
		exp->child[0]=oper;	
		if(tt==EQUAL)
		 {
			exp->nodek=expk;
			exp->kind.exp=opk;
			exp->attr.op=tt;
			pt=tt;
			tt=gettoken();
		 }
		exp->child[1]=simple_expression(pt,stab,ltab);	
	  }
	 else
		errhandle(44);
	return exp;
 }

struct treenode * simple_expression(enum tokentype pt,struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * se,*ses=NULL;
//	print(stab);
//	print(ltab);
	flag=0;
	se=additive_expression(pt,stab,ltab);
//	tt=gettoken();
	if(tt==LE || tt==LT || tt== GT || tt==GE || tt==EQ || tt== NE)
	 {
		ses=(struct treenode *)malloc(sizeof(struct treenode));
		ses->child[0]=ses->child[1]=ses->child[2]=NULL;
		ses->child[0]=se;
		ses->nodek=expk;
		ses->kind.exp=opk;
		ses->attr.op=tt;
		flag=0;
		pt=tt;
		tt=gettoken();
		ses->child[1]=additive_expression(pt,stab,ltab);
	 }
	else
		ses=se;
	return ses;
 }

struct treenode * additive_expression(enum tokentype pt,struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * ae=NULL,* temp;
	int flag1;
	//print(stab);
	//print(ltab);
	do
	 {
		flag1=1;
		if(!ae)
			ae=term(pt,stab,ltab);
		else
			ae->child[1]=term(pt,stab,ltab);
		if(tt==PLUS || tt==MINUS)
		 {
			temp=(struct treenode *)malloc(sizeof(struct treenode));
			temp->child[0]=temp->child[1]=temp->child[2]=NULL;
			temp->nodek=expk;
			temp->kind.exp=opk;
			temp->attr.op=tt;
			temp->child[0]=ae;
			ae=temp;
			flag1=0;
			tt=gettoken();
		 }
	 }while(!flag1);
	return ae;
 }

struct treenode * term(enum tokentype pt,struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * te=NULL,* temp;
	int flag2;
//	print(stab);
//	print(ltab);
	do
	 {
		flag2=1;
		if(!te)				// te-> NULL
			te=factor(pt,stab,ltab);
		else
			te->child[1]=factor(pt,stab,ltab);
	 	if(tt==ASTERISK || tt==SLASH || tt== MOD)
		 {
			temp=(struct treenode *)malloc(sizeof(struct treenode));
			temp->child[0]=temp->child[1]=temp->child[2]=NULL;
			temp->nodek=expk;
			temp->kind.exp=opk;
			temp->attr.op=tt;
			temp->child[0]=te;
			te=temp;
			flag2=0;
			tt=gettoken();
		 }
	 }while(!flag2);
	return te;
 }

struct treenode * factor(enum tokentype pt,struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * fc=NULL;
	int flg=0;
	if(tt==MINUS)
	 {
		flg=1;
		tt=gettoken();
	 }
	if(tt==OBR)		// void functions    check if necessary
	 {
		tt=gettoken();
		if(pt==ID)
		 {}
			//args();
		else
			expression(stab,ltab);
		if(tt==CBR)
		 {
			tt=gettoken();
			return ;
 		 }
		else
			errhandle(22);
	 }
	else if(tt==DIGIT)
	 {
		fc=(struct treenode *)malloc(sizeof(struct treenode));
		fc->child[0]=fc->child[1]=fc->child[2]=NULL;
		fc->sibling=NULL;
		fc->nodek=expk;
		fc->kind.exp=digk;
		fc->attr.val=atoi(curtok);			//current token value
		if(flg)
			fc->attr.val*=-1;
		tt=gettoken();
		return fc;
	 }
	else if(tt==ID && !flg)
	 {
		char ptok[32];
		fc=(struct treenode *)malloc(sizeof(struct treenode));
		fc->child[0]=fc->child[1]=fc->child[2]=NULL;
		fc->nodek=expk;
		strcpy(ptok,curtok);			//move current token value tp ptok    check!
		tt=gettoken();
		if(tt==OBB)
		 {
			enum tokentype pt=tt;
			tt=gettoken();
			fc->child[0]=additive_expression(pt,stab,ltab);
		 	if(tt==CBB)
			 {
				fc->kind.exp=arrk;
				fc->attr.name=(char *)malloc(32);
				strcpy(fc->attr.name,ptok);
				tt=gettoken();	
			 }
			else
				errhandle(23);
		 }
		else if(tt==OBR)
		 {
			int tmp=0;	
			fc->kind.exp=funk;
			fc->attr.fun.name=(char *)malloc(32);
			strcpy(fc->attr.fun.name,ptok);			//global var curtok
			tt=gettoken();
			if(tt!=CBR)
			 {	
				flag2=0;
				fc->child[0]=args(&tmp,stab,ltab);
				fc->attr.fun.narg=tmp;
		//		print(stab);
		//		printf("%d",fc->attr.fun.narg);
				if(!lib(fc) && !lookup(stab,fc))			// check fun definition
					errhandle(46);
			 }
			if(tt==CBR)
			 {
				tt=gettoken();
			 }
			else
				errhandle(24);
		 }
		else
		 {
	//		print(stab);
	//		print(ltab);
			fc->kind.exp=idk;
			fc->attr.name=(char *)malloc(32);
			strcpy(fc->attr.name,ptok);			//current token value     check!
			if(!lookup(ltab,fc) && !lookup(stab,fc))
				errhandle(38);
		 }
		return fc;
 	 }
	else
		return NULL;
 }

struct treenode * args(int *count,struct symtab *stab,struct symtab *ltab)
 {
	struct treenode * fir,* niete;
	int chance=0;
	enum tokentype pt=tt;
	do
	 {
		if(!chance++)
		 {
			/*if(flag2++)
			 {
				pt=tt;
				tt=gettoken();
			 }*/
			//chance=1;
			fir=additive_expression(pt,stab,ltab);		
			niete=fir;
		 }
		else
		 {
		//	if(flag2++)
			 {
				pt=tt;
				tt=gettoken();
			 }
			if(niete)
			 {
				niete->sibling=additive_expression(pt,stab,ltab);
				niete=niete->sibling;
			 }
		 }
	 }while(tt==COMMA);
	*count=chance;
	niete->sibling=NULL;
	return fir;
 }
			
void errhandle(int n)
 {
	printf("\nerror: %d \n",n);
	switch(n)
	 {
		case 1:
		case 2:
		case 11:
		case 12:
		case 21:
		case 23:
			printf("incorrect array declaration\n");
			break;
		case 3:
			printf("incorrect function declaration\n");
			break;
		case 4:
		case 5:
		case 6:
		case 13:
			printf("incorrect declaration\n");
			break;
		case 7:
		case 8:
		case 25:
		case 26:
			printf("incorrect parameter list\n");
			break;
		case 10:
			printf("opening curly brace missing ay the beginning of compound statement\n");
			break;
		case 14:
			printf("illegal statement\n");
			break;
		case 15:
		case 20:
			printf("semi colon missing\n");
			break;
		case 16:
		case 18:
		case 24:
			printf("closing brace missing\n");
			break;
		case 17:
		case 19:
			printf("opening brace missing\n");
			break;
		case 30:
			printf("function should not return a value\n");
			break;
		case 31:
			printf("function should return a value\n");
			break;
		case 33:
		case 35:
		case 36:
			printf("variable redeclaration\n");
			break;
		case 38:
			printf("variable not defined\n");
			break;
		case 44:
			printf("illegal expression\n");
			break;
		case 46:
			printf("reference to undefined function\n");
			break;
	 }
	exit(0);
 } 
