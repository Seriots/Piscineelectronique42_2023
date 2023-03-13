#include <stdio.h>
int main()
{
	int a = 0;
	int	i = 0;
	int b = 255;

	while (1)
	{
		b &= ~(1 << 3);
		printf("a: %d\n", b);
		while (i < 50000000)
			i++;
		i=0;
		b |= (1<<3);
		printf("a: %d\n", b);
		while (i < 50000000)
			i++;
		i=0;
	}
}