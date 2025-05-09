/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:20:40 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 22:19:18 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutex(t_program *simulation);
static int	initialize_simulation(t_program *simulation);
static void	initialize_philos(t_program *simulation);
static int	setup_threads(t_program *simulation);

int	start_simulation(t_program *simulation)
{
	if (initialize_simulation(simulation))
		return (1);
	if (setup_threads(simulation))
		return (1);
	if (simulation->stop_flag == DEAD)
		printf("%ld %d died\n", simulation->death_time, simulation->dead_philo);
	if (simulation->stop_flag == STOP)
		printf("simulation done!\n");
	return (0);
}

static int	initialize_simulation(t_program *simulation)
{
	simulation->mut = 0;
	simulation->start_time = get_time();
	simulation->philos = NULL;
	simulation->forks = NULL;
	simulation->philos = malloc(sizeof(t_philo) * simulation->philos_num);
	if (!simulation->philos)
		return (printf("Error\nMalloc failed!\n"), 1);
	simulation->forks = malloc(sizeof(pthread_mutex_t) * \
		simulation->philos_num);
	if (!simulation->forks)
	{
		free(simulation->philos);
		simulation->philos = NULL;
		return (printf("Error\nMalloc failed!\n"), 1);
	}
	simulation->stop_flag = 0;
	simulation->dead_philo = 0;
	initialize_philos(simulation);
	if (init_mutex(simulation))
		return (1);
	return (0);
}

static int	setup_threads(t_program *simulation)
{
	int			i;
	pthread_t	monitor;

	i = -1;
	while (++i < simulation->philos_num)
	{
		if (pthread_create(&simulation->philos[i].thread, NULL, \
			philosopher, &simulation->philos[i]))
			return (cleanup_threads(simulation, i), 1);
	}
	if (pthread_create(&monitor, NULL, \
		monitor_func, simulation) != 0)
		return (cleanup_threads(simulation, simulation->philos_num), 1);
	i = -1;
	while (++i < simulation->philos_num)
	{
		if (pthread_join(simulation->philos[i].thread, NULL))
			return (printf("Error\nThread join failed!\n"), 1);
	}
	pthread_join(monitor, NULL);
	return (0);
}

static void	initialize_philos(t_program *simulation)
{
	int	i;

	i = -1;
	while (++i < simulation->philos_num)
	{
		simulation->philos[i].last_meal_time = get_time();
		simulation->philos[i].id = i + 1;
		simulation->philos[i].meals_eaten = 0;
		simulation->philos[i].fork_r = i;
		simulation->philos[i].fork_l = (i + 1) % simulation->philos_num;
		simulation->philos[i].simulation = simulation;
	}
}

static int	init_mutex(t_program *simulation)
{
	if (init_forks_mutex(simulation))
		return (1);
	if (pthread_mutex_init(&simulation->log_lock, NULL))
		return (printf("Error\nMutex init failed!\n"), 1);
	simulation->mut |= LOG;
	if (pthread_mutex_init(&simulation->meal_lock, NULL))
		return (printf("Error\nMutex init failed!\n"), 1);
	simulation->mut |= MEAL;
	if (pthread_mutex_init(&simulation->mealtime_lock, NULL))
		return (printf("Error\nMutex init failed!\n"), 1);
	simulation->mut |= TIME;
	if (pthread_mutex_init(&simulation->died_lock, NULL))
		return (printf("Error\nMutex init failed!\n"), 1);
	simulation->mut |= DIED;
	return (0);
}
