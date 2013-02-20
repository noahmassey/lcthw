#include <stdio.h>

void for_loop(int count, int *ages, char **names)
{
	printf("---for_loop\n");
	int i = 0;
	for (i = 0; i < count; i++) {
		printf("%s is %d years old.\n",
				names[i], ages[i]);
	}
	printf("---\n");
}

void while_loop(int count, int *ages, char **names)
{
	printf("---while_loop\n");
	int i = 0;
	while (i < count) {
		printf("%s is %d years old.\n",
				names[i], ages[i]);
		i++;
	}
}

void get_addresses(int count, int *ages, char **names)
{
	printf("---get_addresses\n");
	int i = 0;
	for (i = 0; i < count; i++) {
		printf("%s [address: %p] is %d [address: %p] years old.\n",
				names[i], &names[i], ages[i], &ages[i]);
	}
	printf("---\n");
}

int main(int argc, char *argv[])
{
	// create two arrays we care about
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {
		"Alan",
		"Frank",
		"Mary",
		"John",
		"Lisa",
	};

	// setup the pointers to the start of the arrays
	int *cur_age = ages;
	char **cur_name = names;

	// safely get the size of ages
	int count = sizeof(ages) / sizeof(int);

	for_loop(count, cur_age, cur_name);
	while_loop(count, cur_age, cur_name);
	get_addresses(count, cur_age, cur_name);

	return 0;
}
