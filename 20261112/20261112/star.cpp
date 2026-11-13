#include <stdio.h>

int main() {

	for (int i = 0;i <= 5;i++) {
		for (int j = 0;j < i;j++) {
			printf("*");
		}
		printf("\n");
	}


	int i = 0;
	while (i <= 5) {
		int j = 0;
		while (j < i) {
			printf("*");
			j++;
		}
		printf("\n");
		i++;
	}
}

