enum tokentype 
 {
	IF=256,		//if 256
	ELSE,		//else 257
	INT,		//int 258
	RETURN,		//return 259
	VOID,		//void 260
	WHILE,		//while 261
	ID,		//identifier 262
	DIGIT,		//integer constant 263
	PLUS,		//+ 264
	MINUS,		//- 265
	ASTERISK,	//* 266
	SLASH,		/// 267
	MOD,		//% 268
	NOT,		//! 269
	INC,		//++ 270
	DEC,		//-- 271
	LT,		//< 272
	LE,		//<= 273
	GT,		//> 274
	GE,		//>= 275
	EQ,		//== 276
	NE,		//!= 277
	EQUAL,		//= 278
	SEMI,		//; 279
	COMMA,		//, 280
	OBR,		//( 281
	CBR,		//) 282
	OBB,		//[ 283
	CBB,		//] 284
	OCB,		//{ 285
	CCB,		//} 286
	ENDOFFILE,      // 287	
	ERROR		// 288
 };

char resword[6][7]={"else","if","int","return","void","while"};

char libr[4][7]={"read","print","power","fact"};

enum tokentype type[6]={ELSE,IF,INT,RETURN,VOID,WHILE};

char *curtok,str[32];

FILE *f;

int mainflag;
