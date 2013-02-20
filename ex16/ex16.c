#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Declare the Person structure
struct Person {
	char *name;
	int age;
	int height;
	int weight;
};

// Create and return a Person structure
struct Person *Person_create(char *name, int age, int height, int weight)
{
	// Obtain a chuck of raw memory the size of the Person structure
	struct Person *who = malloc(sizeof(struct Person));

	// Ensure malloc gave us a good chunk of memory
	assert(who != NULL);

	// Set values on the person
	// Note: use strdup to ensure the value of char *name actually
	// gets copied to our new chunk of memory (strdup returns a pointer to a new char *)
	who->name = strdup(name);
	who->age = age;
	who->height = height;
	who->weight = weight;

	return who;
}

// Create and return a Person structure (on the STACK)
// (extra credit)
struct Person Person_create_stack(char *name, int age, int height, int weight)
{
	struct Person who;
	who.name = name;
	who.age = age;
	who.height = height;
	who.weight = weight;

	return who;
}

// Destory a Person structure (freeing memory)
void Person_destroy(struct Person *who)
{
	assert(who != NULL);

	// free() will free the memory space pointed to by these pointers
	free(who->name);
	free(who);
}

// Print out a Person structure
void Person_print(struct Person *who)
{
	printf("Name: %s\n", who->name);
	printf("\tAge: %d\n", who->age);
	printf("\tHeight: %d\n", who->height);
	printf("\tWeight: %d\n", who->weight);
}

// Print out a Person structure (STACK)
void Person_print_stack(struct Person who)
{
	printf("Name: %s\n", who.name);
	printf("\tAge: %d\n", who.age);
	printf("\tHeight: %d\n", who.height);
	printf("\tWeight: %d\n", who.weight);
}

int main(int agrc, char *argv[])
{
	// make two people structures
	struct Person *joe = Person_create(
			"Joe Alex", 32, 64, 140);

	struct Person *frank = Person_create(
			"Frank Blank", 20, 72, 180);

	// print them out and where they are in memory
	printf("Joe is at memory location: %p\n", joe);
	Person_print(joe);

	printf("Frank is at memory location: %p\n", frank);
	Person_print(frank);

	// make everyone 20 years older and print them again
	joe->age += 20;
	joe->height -= 2;
	joe->weight += 40;
	Person_print(joe);

	frank->age += 20;
	frank->weight += 20;
	Person_print(frank);

	// destroy them both so we clean up
	printf("\nDestroying joe and frank\n");
	Person_destroy(joe);
	Person_destroy(frank);

	// Person on the stack
	struct Person dan = Person_create_stack(
			"Dan Jarvis", 28, 70, 165);

	printf("Dan is at memory location: %p\n", &dan);
	Person_print_stack(dan);

	// Since dan was created on the stack, we do not need to manually destroy him

	return 0;
}
