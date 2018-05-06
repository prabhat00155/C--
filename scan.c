//#include "global.h"

//char *curtok;

int gettoken()
 {
	int flag=0;
	char ch;
	while(!flag)
	 {
		ch=fgetc(f);
		if(ch==EOF)
			flag=1;
		if(!flag)
		 {
			switch(ch)
		 	 {
				case ' ':		//if blank space
				case '\n':
				case '\t':
					break;		//do nothing and scan forward
				case '/':
					ch=fgetc(f);
					if(ch=='*') 	//comment
					 {
						int go=0;
						while(!go)
						 {
							go=1;
							do
						 	 {
								ch=fgetc(f);
								if(ch==EOF)
									return ERROR;
						 	 }while(ch!='*');
							while(ch=='*')
								ch=fgetc(f);
							if(ch!='/')
						 		go=0;
						 }
						break;
					 }
					else		//division
					 {
						ungetc(ch,f);
						return SLASH;
					 }
				case '<':
					ch=fgetc(f);
					if(ch=='=')
						return LE;
					else
					 {
						ungetc(ch,f);
						return LT;
					 }
				case '>':
					ch=fgetc(f);
					if(ch=='=')
						return GE;
					else
					 {
						ungetc(ch,f);
						return GT;
					 }
				case '=':
					ch=fgetc(f);
					if(ch=='=')
					 	return EQ;
					else
					 {
						ungetc(ch,f);
						return EQUAL; 
					 }
				case '!':
					ch=fgetc(f);
					if(ch=='=')
						return NE;
					else
					 {
						ungetc(ch,f);
						return NOT;
					 }
				case '+':
					ch=fgetc(f);
					if(ch=='+')
						return INC;
					else
					 {
						ungetc(ch,f);
						return PLUS;
					 }
				case '-':
					ch=fgetc(f);
					if(ch=='-')
						return DEC;
					else
					 {
						ungetc(ch,f);
						return MINUS;
					 }
				case '*':
					return ASTERISK;
				case '%':
					return MOD;
				case ';':
					return SEMI;
				case ',':
					return COMMA;
				case '(':
					return OBR;
				case ')':
					return CBR;
				case '[':
					return OBB;
				case ']':
					return CBB;
				case '{':
					return OCB;
				case '}':
					return CCB;
			        default:
					if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || ch=='_')		//identifier
					 {
						//char str[32];
						int i=0;
						enum tokentype type;
						while((ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || ch=='_' || (ch>='0' && ch<='9')) 
						 {
							//fprintf(st,"%c",ch);	
							str[i++]=ch; 
							ch=fgetc(f);
						 }
						str[i]='\0';
						ungetc(ch,f);
						if(!(type=keyword(str)))
						 {
							FILE *st=fopen("symtab.c","a");
							fprintf(st,"%s",str);
							fprintf(st,"%c",'\n');
							fclose(st);
									// copy id into global var curtok
							curtok=str;
					//		printf("%s\n",curtok);
							return ID;
						 }
						else
						 return type;
					 }
					else if(ch>='0' && ch<='9')
					 {
						FILE *st=fopen("symtab.c","a");
						int i=0;
					//	char cur[32];
						while(ch>='0' && ch<='9')
						 {
							str[i++]=ch;				//copy digit to curtok
							fprintf(st,"%c",ch);	 
							ch=fgetc(f);
						 }
						str[i]='\0';
						curtok=str;
					//	printf("%d\n",atoi(curtok));
						ungetc(ch,f);
						fprintf(st,"%c",'\n');
						fclose(st);
						return DIGIT;
					 }
					else
					 return ERROR;
			 }				
						
		 } 
				
		
				
	 }		
	return ENDOFFILE;	
 }
