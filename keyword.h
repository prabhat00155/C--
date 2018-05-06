

enum tokentype keyword(char *s)
 {
	int first=0,last=5,middle,val;
	while(first<=last)
	 {	
		middle=(first+last)/2;
		val=strcmp(s,resword[middle]);
		if(!val)
			return type[middle];
  		else if(val>0)
			first=middle+1;
		else
			last=middle-1;
	 }
	return 0;
 }	
		
