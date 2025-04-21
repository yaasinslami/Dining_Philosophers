/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:16:09 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 16:48:19 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	cleanup_childsem(t_program *sim)
{
	int	i;

	i = -1;
	if (sim->info & PHILOS)
	{
		while (++i < sim->philos_num)
		{
			sem_close(sim->philos[i].meal_sem);
			sem_close(sim->philos[i].mealtime_sem);
		}
	}
}

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
				break ;
		}
		else if (WIFSIGNALED(status))
			break ;
		if (i == simulation->philos_num)
			i = 0;
		i++;
	}
	kill_world(simulation);
}

void	kill_world(t_program *simulation)
{
	int	i;

	if (!simulation)
		return ;
	i = 0;
	if (simulation->info & PHILOS)
	{
		while (i < simulation->philos_num)
		{
			if (simulation->philos[i].philo_pid > 0)
				kill(simulation->philos[i].philo_pid, SIGKILL);
			i++;
		}
	}
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	sem_close(simulation->forks);
	sem_close(simulation->log_sem);
	cleanup_childsem(simulation);
	if (simulation->philos)
		free(simulation->philos);
	free(simulation);
	exit (0);
}
