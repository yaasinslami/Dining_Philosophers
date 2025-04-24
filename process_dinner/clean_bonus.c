/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:16:09 by yslami            #+#    #+#             */
/*   Updated: 2025/04/24 02:05:16 by yslami           ###   ########.fr       */
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

static int	handle_status(t_program *sim, int status)
{
	int	code;

	if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
		if (code != 0)
		{
			sem_post(sim->end_sem);
			return (1);
		}
	}
	else if (WIFSIGNALED(status))
		return (1);
	return (0);
}

void	wait_philos(t_program *sim)
{
	int		status;
	pid_t	pid;

	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (handle_status(sim, status))
			break ;
		usleep(1000);
	}
	kill_world(sim);
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
	sem_close(simulation->forks);
	sem_close(simulation->log_sem);
	sem_close(simulation->done_sem);
	sem_close(simulation->end_sem);
	cleanup_childsem(simulation);
	if (simulation->philos)
		free(simulation->philos);
	free(simulation);
	exit (0);
}
