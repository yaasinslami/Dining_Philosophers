/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 02:47:18 by yslami            #+#    #+#             */
/*   Updated: 2025/04/22 03:04:23 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	*check_end_signal(void *arg)
{
	t_program	*sim;

	sim = (t_program *)arg;
	sem_wait(sim->end_sem);
	kill_world(sim);
	return (NULL);
}

static void	*wait_all_meals(void *arg)
{
	t_program	*sim;
	int			i;
	pthread_t	exit_thread;

	sim = (t_program *)arg;
	i = 0;
	pthread_create(&exit_thread, NULL, check_end_signal, sim);
	pthread_detach(exit_thread);
	while (i < sim->philos_num)
	{
		sem_wait(sim->done_sem);
		i++;
	}
	sem_wait(sim->log_sem);
	printf("Simulation done!\n");
	sem_post(sim->end_sem);
	return (NULL);
}

void	done_monitor(t_program *sim)
{
	pthread_t	done_thread;

	if (sim->number_of_eats != -1)
	{
		if (pthread_create(&done_thread, NULL, wait_all_meals, sim))
		{
			printf("Failed to create meal monitor thread\n");
			kill_world(sim);
		}
		pthread_detach(done_thread);
	}
}
