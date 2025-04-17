/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:16:09 by yslami            #+#    #+#             */
/*   Updated: 2025/04/17 13:34:38 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wait_philos(t_program *simulation)
{
	int		status;
	pid_t	pid;
	int		i;
	int		exit_code;

	i = 0;
	while (i < simulation->philos_num)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WIFEXITED(status))
		{
			exit_code = WEXITSTATUS(status);
			if (exit_code != 0)
				kill_world(&simulation);
		}
		if (i == simulation->philos_num)
			i = 0;
		i++;
	}
	kill_world(&simulation);
}

void	kill_world(t_program **simulation)
{
	int	i;

	if (!simulation || !*simulation)
		return ;
	i = 0;
	while (i < (*simulation)->philos_num)
	{
		if ((*simulation)->philos[i].philo_pid > 0)
			kill((*simulation)->philos[i].philo_pid, SIGKILL);
		i++;
	}
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	sem_close((*simulation)->forks);
	sem_close((*simulation)->log_sem);
	if ((*simulation)->philos)
		free((*simulation)->philos);
	free(*simulation);
	exit (0);
}
