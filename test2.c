int x;

int sum(int a,int b)
 {
	return a+b;
 }

int sum(int a,int b,int c)
 {
	return a+b+c;
 }	

int main()
 {
	x=sum(3,7);
	print(x);
	x=sum(4,5,7);
	print(x);
 }
