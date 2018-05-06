
enum nodekind
 {
	deck,stmtk,expk
 };

enum deckind
 {
	prgdec,vardec,fundec,blkdec,arrdec
 };

enum stmtkind
 {
	expstmt,compstmt,selstmt,iterstmt,retstmt
 };

enum expkind
 {
	opk,idk,digk,funk,arrk
 };


#define MAXCHLD 3

struct arg
 {
	char * info;
	struct arg * next;
 };

struct treenode
 {
	struct treenode * child[MAXCHLD];
	struct treenode * sibling;
	int lineno;
	enum nodekind nodek;
	union 
	 {
		enum deckind dec;
		enum stmtkind stmt;
		enum expkind exp;
	 }kind;
	union
	 {
		enum tokentype op;
		int val;
		char * name;
		struct
		 {
			char *name;
			int narg;
			enum {voidt,intt} rettype;
			struct arg *list;
		 }fun;
		struct
		 {
			char *name;
			int size;
		 }arr;
	 }attr;
 };
