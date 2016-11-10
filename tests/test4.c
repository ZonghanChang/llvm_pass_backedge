
int main() {
	int a = 1;
	int b = 2;
	int c;
	int d;
	int e;
L0: 
	c = a + b; 
	d = c - a;
 	if (c < d) goto L2;
L1: 
 	d = b + d;
 	if (d < 1) goto L3;
L2: 
 	b = a + b;
 	e = c - a;
 	if (e == 0) goto L0;
 	
 	a = b + d;
 	b = a - d;
 	goto L4;
L3: 
 	d = a + b; 
 	e = e + 1;
L4: 
 	return 0;
}
