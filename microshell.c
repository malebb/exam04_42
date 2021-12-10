/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 11:33:03 by mlebrun           #+#    #+#             */
/*   Updated: 2021/12/10 11:50:42 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

unsigned int	count_cmd_group(char *argv, unsigned int i)
{
	while (argv[i] != NULL)
	{
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	unsigned int	i;
	unsigned int	j;
	t_data			data;

	i = 0;
	while (argv[i] != NULL)
	{
		data.nb_cmd_group = count_cmd_group(argv, i);
		while (j < data.nb_cmd_group)
		{
			j++;
		}
		i++;
	}
	return (0);
}
