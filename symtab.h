struct symtab
 {
	char name[32];
	enum deckind decl;
	union
	 {
		int size;
		int noofarg;
	 }attr;	
	struct symtab *link;
 };

int count(struct arg *ff)
 {
	struct arg *f=ff;
	int count=0;
	while(f)
	 {	
		f=f->next;
		count++;
	 }
	return count;
 }

int lookup(struct symtab *ff,struct treenode *pp)			// scan through the symbol table
 {
	struct symtab *f=ff;
	struct treenode *p=pp;
	while(f)
	 {
		if(f->decl==fundec)
		 {
			if((p->nodek==deck && f->decl==p->kind.dec) || (p->nodek==expk && p->kind.dec==funk))
			 {
				if(!strcmp(f->name,p->attr.fun.name))
				 {
					if(f->attr.noofarg==p->attr.fun.narg)
						return 1;		// function already defined
				 }
			 }	
		 }
		else if(f->decl==vardec)
			if(!strcmp(f->name,p->attr.name))
				return 1;				// variable already defined	
		else if(f->decl==arrdec)
			if(!strcmp(f->name,p->attr.arr.name))		// array already defined
				return 1;
		f=f->link;
	 }
	return 0;			// not defined
 }

/*int flookup(struct symtab *ff,struct treenode *pp,int cc)
 {
	struct symtab *f=ff;
	struct treenode *p=pp;
	while(f)
	 {
		if(f->decl==fundec)
			if(!strcmp(f->name,p->attr.fun.name))
				if(f->attr.noofarg==cc)
					return 1;		// function already defined	
		f=f->link;
	 }
	return 0;
 }*/

void insert(struct symtab *ff,struct treenode *pp)			// insert into the symbol table
 {
	struct symtab *k,*f=ff;
	struct treenode *p=pp;
	if(!strcmp(f->name,"\0") && f->attr.size==-1)
		k=f;
	else
		k=(struct symtab *)malloc(sizeof(struct symtab));
	k->decl=p->kind.dec;
	if(k->decl==arrdec)
	 {
		k->attr.size=p->attr.arr.size;		// check!
		strcpy(k->name,p->attr.arr.name);
	 }
	else if(k->decl==fundec)
	 {
	//	k->attr.noofarg=count(p->attr.fun.list);	// count() count no. of args
		strcpy(k->name,p->attr.fun.name);
		k->attr.noofarg=p->attr.fun.narg;
	 }
	else
		strcpy(k->name,p->attr.name);
	k->link=NULL;
	if(k!=f)
	 {
		if(f)
 		 {
			while(f->link)
	 		 {
				f=f->link;
			 }
			f->link=k;
		 }
		else
			f=k;
	 }
 }

void insarg(struct symtab *ff,struct arg *pp)
 {
	struct symtab *f=ff;
	struct arg *p=pp;
	if(p)
	 {
		f->decl=vardec;
		strcpy(f->name,p->info);
		p=p->next;
		while(p)
		 {
			f->link=(struct symtab *)malloc(sizeof(struct symtab));
			f=f->link;
			f->decl=vardec;
			strcpy(f->name,p->info);
			p=p->next;
		 }
	 }
	else
	 {
		f->attr.size=-1;
		strcpy(f->name,"\0");
	 }
	f->link=NULL;
 }

void del(struct symtab *f)
 {
	struct symtab *temp;
	while(f)
	 {
		temp=f;
		f=f->link;
		free(temp);
	 }
 }

void print(struct symtab *ff)
 {
	struct symtab *f=ff;	
	while(f)
	 {
		printf("\n%s	%u",f->name,f->decl);
		if(f->decl==fundec)
			printf("	%d\n",f->attr.noofarg);
		f=f->link;
	 }
 }

int lib(struct treenode *r)
 {
	int first=0,last=3,middle,val;
	while(first<=last)
	 {	
		middle=(first+last)/2;
		val=strcmp(r->attr.fun.name,libr[middle]);
		if(!val)
			return 1;	//library routine
  		else if(val>0)
			first=middle+1;
		else
			last=middle-1;
	 }
	return 0;	
 }
