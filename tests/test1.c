#include <stdio.h>
//struct A {
//	
//};
//int foo(int, struct A);
//void bar();
//int main() {
//	
//}
int main() {
	int n;
	int sum;
	sum = 0;
	for (n = 0; n < 10; n++)
		sum = sum + n*n;
	printf("sum: %d\n", sum);
}

//int foo(int x, struct A a) {
//	int i = 1;
//	bar(5,6,7,8);
//	return 0;
//}
//
//void bar(int a, int b, int c, int d) {
//	
//}
