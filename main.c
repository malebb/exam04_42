/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 18:45:14 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/30 10:57:10 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

void	wait_cmds_execution(int *pids, unsigned int nb_cmd)
{
	unsigned int	i;

	i = 0;
	while (i < nb_cmd)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

unsigned int	count_arg(char **argv, int i)
{
	unsigned int	count;

	count = 0;
	if (argv[i])
		i++;
	while (argv[i] != NULL && strcmp(argv[i], ";") != 0
			&& strcmp(argv[i], "|") != 0)
	{
		i++;
		count++;
	}
	return (count);
}

unsigned int	ft_strlen(char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strdup(char *str)
{
	char			*new_str;
	unsigned int	len;
	unsigned int	i;

	len = ft_strlen(str);
	new_str = malloc(sizeof(char) * (len + 1));
	i = 0;
	while (str[i] != '\0')
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	**stock_args(char **argv, int *i)
{
	char			**args;
	unsigned int	arg_nb;
	unsigned int	j;

	arg_nb = count_arg(argv, *i);
	args = malloc(sizeof(char *) * (arg_nb + 2));
	if (!args)
		return (NULL);
	j = 0;
	args[j] = argv[*i];
	*i = *i + 1;
	j++;
	while (argv[*i] != NULL && strcmp(argv[*i], ";") != 0
			&& strcmp(argv[*i], "|") != 0)
	{
		args[j] = ft_strdup(argv[*i]);
		j++;
		*i = *i + 1;
	}
	args[j] = NULL;
	return (args);
}

void	print_cmd(char *path, char **args)
{
	unsigned int	i;

	printf("path = %s\n", path);
	i = 0;
	while (args[i] != NULL)
	{
		printf("arg: %s\n", args[i]);
		i++;
	}
}

unsigned int	count_cmd(char **argv)
{
	unsigned int	count;
	unsigned int	i;

	count = 0;
	i = 1;
	while (argv[i] != NULL)
	{
		if (strcmp(argv[i], "|") == 0)
			i++;
		else
		{
			i += count_arg(argv, i)	+ 1;
			count++;
		}
	}
	return (count);
}

int		**init_pipe(unsigned int nb_cmd)
{
	int		**pipe_fd;
	unsigned int	i;

	pipe_fd = malloc(sizeof(int*) * nb_cmd - 1);
	if (!pipe_fd)
		return (NULL);
	i = 0;
	while (i < nb_cmd - 1)
	{
		pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!pipe_fd[i])
			return (NULL);
		pipe(pipe_fd[i]);
		i++;
	}
	return (pipe_fd);
}

int		main(int argc, char **argv, char **envp)
{
	int					i;
	int					j;
	char				**args;
	char				*path;
	int					*pids;
	unsigned int		nb_cmd;
	int					**pipe_fd;

	i = 1;
	j = 0;
	nb_cmd = count_cmd(argv);
	pids = malloc(sizeof(int) * nb_cmd);
	pipe_fd = init_pipe(nb_cmd);
	while (argv[i] != NULL)
	{
		pids[j] = fork();
		if (pids[j] == 0)
		{
			if (j == 0)
			{
				if (argv[i + count_arg(argv, i) + 1]
					&& strcmp(argv[i + count_arg(argv, i) + 1], "|") == 0)
					dup2(pipe_fd[j][1], 1);
			}
			else
			{
				dup2(pipe_fd[j - 1][0], 0);
				if (argv[i + count_arg(argv, i) + 1]
					&& strcmp(argv[i + count_arg(argv, i) + 1], "|") == 0)
				{
					dup2(pipe_fd[j][1], 1);
				}
			}
			path = strdup(argv[i]);
			args = stock_args(argv, &i);
			execve(path, args, envp);
			printf("Error\n");
			return (0);
		}
		j++;
		i += count_arg(argv, i) + 1;
		if (argv[i] && strcmp(argv[i], "|") == 0)
			i++;
	}
	wait_cmds_execution(pids, nb_cmd);
	return (0);
}
