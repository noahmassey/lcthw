#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("ERROR: You need one argument.\n");
		return 1;
	}

	int i = 0;
	char letter = argv[1][i];
	for (i = 0; argv[1][i] != '\0'; i++, letter = argv[1][i]) {
		if (letter < 65 || (letter > 90 && letter < 97) || letter > 122) {
			printf("%c is not a _letter_\n", letter);
			continue;
		}

		// Convert letter to lower case
		if (letter < 97)
			letter += 32;
		switch (letter) {
			case 'a':
				printf("%d: 'a'\n", i);
				break;

			case 'e':
				printf("%d: 'e'\n", i);
				break;

			case 'i':
				printf("%d: 'i'\n", i);
				break;

			case 'o':
				printf("%d: 'o'\n", i);
				break;

			case 'u':
				printf("%d: 'u'\n", i);
				break;

			case 'y':
				if (i > 2) {
					// it's only sometimes Y
					printf("%d: 'y'\n", i);
				}
				break;

			default:
				printf("%d: %c is not a vowel\n", i, letter);
		}
	}

	return 0;
}
