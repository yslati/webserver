#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int	ft_strlen(char *str) {
	int len = 0;

	while (*str++)
		len++;
	return len;
}

void	replace