/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:36:19 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 12:42:47 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	eat_done(t_program *simulation);
static int	starved(t_program *sim);
static int	check_death(t_program *sim, t_philo *philo);

void	*monitor_func(void *arg)
{
	t_program	*sim;

	sim = (t_program *)arg;
	while (1)
	{
		if (starved(sim) || eat_done(sim))
			break ;
		usleep(500);
	}
	return (NULL);
}

static int	eat_done(t_program *simulation)
{
	int	i;

	i = -1;
	while (++i < simulation->philos_num)
	{
		if (!check_number_of_eats(&simulation->philos[i]))
			return (0);
	}
	pthread_mutex_lock(&simulation->died_lock);
	simulation->stop_flag = STOP;
	pthread_mutex_unlock(&simulation->died_lock);
	return (1);
}

static int	starved(t_program *sim)
{
	int	i;

	i = -1;
	while (++i < sim->philos_num)
	{
		if (check_death(sim, &sim->philos[i]))
		{
			pthread_mutex_lock(&sim->died_lock);
			sim->stop_flag = DEAD;
			sim->dead_philo = sim->philos[i].id;
			sim->death_time = get_time() - sim->start_time;
			pthread_mutex_unlock(&sim->died_lock);
			return (1);
		}
	}
	return (0);
}

static int	check_death(t_program *sim, t_philo *philo)
{
	int	died;

	pthread_mutex_lock(&sim->mealtime_lock);
	died = get_time() - philo->last_meal_time > sim->time_to_die;
	pthread_mutex_unlock(&sim->mealtime_lock);
	return (died);
}

int	should_stop(t_program *sim)
{
	int	status;

	pthread_mutex_lock(&sim->died_lock);
	status = sim->stop_flag;
	pthread_mutex_unlock(&sim->died_lock);
	return (status == DEAD || status == STOP);
}
