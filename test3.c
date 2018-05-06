int m;

int fact(int n)
 {
	if(n==0)
		return 1;
	m=fact(n-1)*n;
	return m;
	
 }

int main()
 {
	m=read();
	m=fact(m);
	print(m);
 }
