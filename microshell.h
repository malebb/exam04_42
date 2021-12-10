/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 11:37:03 by mlebrun           #+#    #+#             */
/*   Updated: 2021/12/10 14:06:51 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct	s_data
{
	unsigned int	nb_cmd_group;
	char			*path;
	char			**args;
	char			**envp;
	int				*pids;
	int				pipe_fd[2];
	int				pipe_fd_2[2];
} t_data;

#endif
