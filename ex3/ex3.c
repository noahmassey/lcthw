#include <stdio.h>

int main()
{
	int age = 10;
	int height = 73;
	int foo = height + age;
	const char * str = "I am a string and stuff";

	printf("I am %d years old.\n", age);
	printf("I am %d inches tall.\n", height);
	printf("The value of foo is %d\n", foo);
	printf("\a\n");
	printf("vertical \v tab\n");
	printf("args order? %2$d and %1$d\n", age, height);
	printf("hex: %08x\n", age);
	printf("%s\n", str);

	return 0;
}
