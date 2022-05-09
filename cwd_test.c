#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	long long num;
	num =  (long long)9223372036854775807;
	unsigned int c = (unsigned char)num;
	printf("%u\n", c);
}
