#include <stdio.h>
#include <stdlib.h>

#define swap(t, x, y) do {t _tmp = (x); (x) = (y); (y) = _tmp;} while(0)

int main(void){
	int x = 10;
	int y = 20;

	printf("before swap x = %d, y = %d\n", x, y);
	
	swap(int, x, y);

	printf("after swap x = %d, y = %d\n", x, y);

	return EXIT_SUCCESS;
}
