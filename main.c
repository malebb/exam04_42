/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 18:45:14 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/28 20:18:20 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define		NB_CMD 2

typedef struct		s_cmd
{
	char	*path;
	int		pipe;
	int		semicolon;
}					t_cmd;

void	wait_cmds_execution(int pids[NB_CMD])
{
	unsigned int	i;

	i = 0;
	while (i < NB_CMD)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

int		main(int argc, char **argv, char **envp)
{
	int		i;
	int		pids[NB_CMD];

	i = 0;
	while (i < NB_CMD)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			return (0);
		}
		i++;
	}
	wait_cmds_execution(pids);
	printf("parent\n");
	return (0);
}
