/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:16:09 by yslami            #+#    #+#             */
/*   Updated: 2025/02/26 11:22:57 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	wait_philos(t_program *simulation)
{
	int	status;
	int	i;

	i = 0;
	while (waitpid(simulation->philos[i].philo_pid, &status, 0) != -1)
	{
		if (status != 0)
		{
			print_logs(simulation->philos, "died");
			kill_world(&simulation);
			return (0);
		}
		if (i == simulation->philos_num)
			i = 0;
		i++;
	}
	kill_world(&simulation);
	return (1);
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
			kill((*simulation)->philos[i++].philo_pid, SIGKILL);
	}
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	sem_close((*simulation)->forks);
	sem_close((*simulation)->log_sem);
	if ((*simulation)->philos)
		free((*simulation)->philos);
	free(*simulation);
	exit(0);
}
