
int main() {
	int a = 1;
	int b = 2;
	int c;
	int d = 0;
L0: 
	c = a + b;
L1:	 
	a = 1;
 	if (a < 1) goto L3;
L2:
 	b = 2;
L3: 
 	a = 2;
L4:  
 	a = a + 1;
 	if (b == a) goto L2;
L5: 
	d = d + 1;
	if(d < 10) goto L5;
 	return 0;
}
