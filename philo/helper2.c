/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:37:05 by yslami            #+#    #+#             */
/*   Updated: 2025/04/20 00:14:18 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_stop(t_program *sim)
{
	int	status;

	status = 0;
	pthread_mutex_lock(&sim->died_lock);
	status = sim->stop_flag;
	pthread_mutex_unlock(&sim->died_lock);
	return (status == STOP);
}

void	cleanup_threads(t_program *simulation, int i)
{
	while (--i > 0)
		pthread_join(simulation->philos[i].thread, NULL);
	printf("Error\nThread creation failed!\n");
}

int check_death(t_program *sim, t_philo *philo)
{
	int	died;

	pthread_mutex_lock(&sim->meal_lock);
	died = get_time() - philo->last_meal_time > sim->time_to_die;
	pthread_mutex_unlock(&sim->meal_lock);
	return (died);
}

int	starved(t_program *sim)
{
	int	i;

	i = -1;
	while (++i < sim->philos_num)
	{
		if (check_death(sim, &sim->philos[i]))
		{
			pthread_mutex_lock(&sim->died_lock);
			sim->stop_flag = STOP;
			sim->dead_philo = sim->philos[i].id;
			sim->death_time = get_time() - sim->start_time;
			pthread_mutex_unlock(&sim->died_lock);
			return (1);
		}
	}
	return (0);
}

void	*monitor_func(void *arg)
{
	t_program	*sim;

	sim = (t_program *)arg;
	while (1)
	{
		if (starved(sim))
			break ;
		usleep(200);
	}
	return (NULL);
}
