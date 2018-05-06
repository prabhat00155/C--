int h;

int maxi(int a,int b)
 {
	int max;
	if(a>b)
		max=a;
	else
		max=b;
	return max;
 }

int maxi(int a,int b,int c)
 {
	int max;
	if(a>b)
	 {
		if(a>c)
			max=a;
		else
			max=c;
	 }
	else
	 {
		if(b>c)
			max=b;
		else
			max=c;
	 }
	return max;
 }

int main()
 {
	int max;
	max=maxi(2,3);
	print(max);
	max=maxi(2,4,3);
	print(max);
 }
