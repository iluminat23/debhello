#include <stdio.h>
#include <debhello_config.h>

int main(int argc, char* argv[])
{
	printf("Hello World: v%s\n", DEBHELLO_VERSION);
	for (int i = 1; i < argc; i++)
		puts(argv[i]);

	return 0;
}
