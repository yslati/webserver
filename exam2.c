#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int ft_strlen(char *str) {
	int len = 0;

	while (*str++)
		len++;
	return (len);
}

void	replace(char **av) {
	int i = -1, j;

	while (av[++i]) {
		i = -1;
		if (strcmp(av[i], ";"))
			while (av[i][j++])
				if (av[i][j] == ';')
					av[i][j] = '*';
	}
}

void	restore(char **av) {
	int i = -1, j;

	while (av[++i]) {
		i = -1;
		while (av[i][j++])
			if (av[i][j] == '*')
				av[i][j] = ';';
	}
}

int		count(char *str, char c) {
	int n = 0, i = -1;

	if (c == ';') {
		while (str[++i])
			if (str[i] == c && str[i + 1])
				n++;
		return n;
	}
	while (str[++i] && str[i != ';'])
		if (str[i] == c && str[i + 1])
			n++;
	return n;
}

void	ft_strjoin(char **cmd, char *str) {
	char *ret = malloc((ft_strlen(cmd[0]) + ft_strlen(str) + 2) * sizeof(char));
	int i = 0, j = 0;

	if (cmd[0]) {
		while (cmd[0][j])
			ret[i++] = cmd[0][j++];
		ret[i++] = ' ';
		free(cmd[0]);
	}
	j = 0;
	while (str[j])
		ret[i++] = str[j++];
	ret[i] = '\0';
	cmd[0] = ret;
}

char*	ft_strdup(char **cmd, char c) {
	char *ret = NULL;
	int i = 0;

	if (cmd[0][0] == ' ')
		cmd[0]++;
	while (cmd[0][i] && cmd[0][i] != c && cmd[0][i] != ';')
		i++;
	ret = malloc((i + 1) * sizeof(char));
	i = -1;
	while (cmd[0][++i] && cmd[0][i] != c && cmd[0][i] != ';')
		ret[i] = cmd[0][i];
	ret[i++] = '\0';
	cmd[0] += i;
	return (ret);
}

char **ft_split(char **str, char c) {
	char **ret = NULL;
	int i = 0, n_space;

	n_space = count(str[0], c);
	ret = malloc((n_space + 1) * sizeof(char *));
	while (i < n_space)
		ret[i++] = ft_strdup(str, c);
	ret[i] = NULL;
	return (ret);
}

