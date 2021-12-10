/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 11:33:03 by mlebrun           #+#    #+#             */
/*   Updated: 2021/12/10 16:14:14 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

unsigned int	count_cmd_group(char **argv, unsigned int i)
{
	unsigned int	count;

	count = 1;
	while (argv[i] != NULL && strcmp(argv[i], ";") != 0)
	{
		if (strcmp(argv[i], "|") == 0)
		{
			count++;
		}
		i++;
	}
	return (count);
}

unsigned int	size_cmd(char **argv, unsigned int i)
{
	unsigned int	size;

	size = 0;
	while (argv[i] != NULL && strcmp(argv[i], "|") != 0
			&& strcmp(argv[i], ";") != 0)
	{
		i++;
		size++;
	}
	return (size);
}

void	init_data(t_data *data, char **envp)
{
	data->pids = NULL;
	data->args = NULL;
	data->envp = envp;
}

void	free_content(t_data data)
{
	free(data.pids);
	free(data.args);
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

void	ft_putstr_fd(char *str, int fd)
{
	write(fd, str, ft_strlen(str));
}

void	fatal(t_data data)
{
	free_content(data);
	ft_putstr_fd("error: fatal\n", 2);
	exit(0);
}

char	**fill_args(char **argv, unsigned int i)
{
	char			**args;
	unsigned int	size;
	unsigned int	j;

	size = size_cmd(argv, i);
	args = malloc(sizeof(char *) * (size + 1));
	j = 0;
	while (j < size)
	{
		args[j] = argv[i];
		i++;
		j++;
	}
	args[j] = NULL;
	return (args);
}

/*
void	print_cmds(t_data data)
{
	unsigned int	i;

	fprintf(stderr, "p: %s\n", data.path);
	i = 0;
	while (data.args[i] != NULL)
	{
		fprintf(stderr, "a: %s\n", data.args[i]);
		i++;
	}
}
*/

void	go_to_next_group(char **argv, unsigned int *i, t_data data)
{
	unsigned int	j;

	j = 0;
	while (j < data.nb_cmd_group)
	{
		*i = *i + size_cmd(argv, *i);
		if (argv[*i] != NULL && strcmp(argv[*i], "|") == 0)
			*i = *i + 1;
		j++;
	}
	if (argv[*i] != NULL && strcmp(argv[*i], ";") == 0)
		*i = *i + 1;

}

int	*init_pids(t_data data)
{
	int	*pids;

	pids = malloc(sizeof(int) * (data.nb_cmd_group));
	if (!pids)
		return (NULL);
	return (pids);
}

void	wait_cmd_group(t_data data)
{
	unsigned int	i;

	i = 0;
	while (i < data.nb_cmd_group)
	{
		waitpid(data.pids[i], NULL, 0);
		i++;
	}
}

void	pipe_cmd(t_data data, char **argv, unsigned int i, unsigned int j)
{
	if ((j % 2) == 0)
	{
		if (j != 0)
		{
			if (dup2(data.pipe_fd_2[0], 0) == -1)
				fatal(data);
		}
		if (argv[i + size_cmd(argv, i)] != NULL
				&& strcmp(argv[i + size_cmd(argv, i)], "|") == 0)
		{
			if (dup2(data.pipe_fd[1], 1) == -1)
				fatal(data);
		}
	}
	else
	{
		if (dup2(data.pipe_fd[0], 0) == -1)
			fatal(data);
		if (argv[i + size_cmd(argv, i)] != NULL
				&& strcmp(argv[i + size_cmd(argv, i)], "|") == 0)
		{
			if (dup2(data.pipe_fd_2[1], 1) == -1)
				fatal(data);
		}
	}
}

void	close_pipes(t_data data, unsigned int j)
{
	if ((j % 2) == 0)
	{
		if (j != 0)
		{
			if (close(data.pipe_fd_2[0]) == -1)
				fatal(data);
			if (close(data.pipe_fd_2[1]) == -1)
				fatal(data);
		}
	}
	else
	{
		if (close(data.pipe_fd[0]) == -1)
			fatal(data);
		if (close(data.pipe_fd[1]) == -1)
			fatal(data);
	}
}

void	cd_cmd(char **argv, unsigned int i)
{
	if (size_cmd(argv, i) != 2)
		ft_putstr_fd("error: cd: bad arguments\n", 2);
	else if (chdir(argv[i + 1]) == -1)
	{
		ft_putstr_fd("error: cd: cannot change directory to ", 2);
		ft_putstr_fd(argv[i + 1], 2);
		ft_putstr_fd("\n", 2);
	}
}

void	exec_cmd(t_data data, char **argv, unsigned int i, unsigned int j)
{
	pipe_cmd(data, argv, i, j);
	close_pipes(data, j);
	if (argv[i] != NULL && strcmp(data.path, "cd") == 0)
		cd_cmd(argv, i);
	else if (argv[i] != NULL && strcmp(argv[i], "|") != 0
		&& strcmp(argv[i], ";") != 0)
	{
		execve(data.path, data.args, data.envp);
		ft_putstr_fd("error: cannot execute ", 2);
		ft_putstr_fd(data.path, 2);
		ft_putstr_fd("\n", 2);
	}
	free_content(data);
	exit(0);
}

void	init_pipes(t_data *data, unsigned int j)
{
	if ((j % 2) == 0)
	{
		if (pipe(data->pipe_fd) == -1)
			fatal(*data);
	}
	else
	{
		if (pipe(data->pipe_fd_2) == -1)
			fatal(*data);
	}
}

void	execute_cd(char **argv, unsigned int i, unsigned int j, t_data data)
{
	cd_cmd(argv, i);
	data.pids[j] = fork();
	if (data.pids[j] == -1)
		fatal(data);
	if (data.pids[j] == 0)
	{
		free_content(data);
		exit(0);
	}
}

void	execute_group_cmd(t_data data, char **argv, unsigned int i)
{
	unsigned int	j;

	j = 0;
	while (j < data.nb_cmd_group)
	{
		data.path = argv[i];
		data.args = fill_args(argv, i);
		if (!data.args)
			fatal(data);
		if (argv[i] != NULL && strcmp(data.path, "cd") == 0
			&& (argv[i + size_cmd(argv, i)] == NULL
			|| strcmp(argv[i + size_cmd(argv, i)], ";") == 0))
			execute_cd(argv, i, j, data);
		else
		{
			init_pipes(&data, j);
			data.pids[j] = fork();
			if (data.pids[j] == -1)
				fatal(data);
			if (data.pids[j] == 0)
				exec_cmd(data, argv, i, j);
			close_pipes(data, j);
		}
		free(data.args);
		data.args = NULL;
		i += size_cmd(argv, i);
		if (argv[i] != NULL && strcmp(argv[i], "|") == 0)
			i++;
		j++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	unsigned int	i;
	t_data			data;

	(void)argc;
	i = 1;
	init_data(&data, envp);
	while (argv[i] != NULL)
	{
		data.nb_cmd_group = count_cmd_group(argv, i);
		data.pids = init_pids(data);
		if (!data.pids)
			fatal(data);
		execute_group_cmd(data, argv, i);
		wait_cmd_group(data);
		go_to_next_group(argv, &i, data);
		free_content(data);
	}
	return (0);
}
