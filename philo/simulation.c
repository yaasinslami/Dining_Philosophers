/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:20:40 by yslami            #+#    #+#             */
/*   Updated: 2025/04/17 14:33:24 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutex(t_program *simulation);
static void	*philosopher(void *data);
static int	initialize_simulation(t_program *simulation);
static void	initialize_philos(t_program *simulation);

int	start_simulation(t_program *simulation)
{
	int	i;

	if (initialize_simulation(simulation))
		return (1);
	i = -1;
	while (++i < simulation->philos_num)
	{
		simulation->philos[i].last_meal_time = get_time();
		if (pthread_create(&simulation->philos[i].thread, NULL, \
			philosopher, &simulation->philos[i]))
		{
			while (--i >= 0)
				pthread_join(simulation->philos[i].thread, NULL);
			return (printf("Error\nThread creation failed!\n"), 1);
		}
	}
	i = -1;
	while (++i < simulation->philos_num)
	{
		if (pthread_join(simulation->philos[i].thread, NULL))
			return (printf("Error\nThread join failed!\n"), 1);
	}
	if (simulation->stop_flag == STOP)
		printf("%ld %d died\n", simulation->death_time, simulation->dead_philo);
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

static void	initialize_philos(t_program *simulation)
{
	int	i;

	i = -1;
	while (++i < simulation->philos_num)
	{
		simulation->philos[i].id = i + 1;
		simulation->philos[i].meals_eaten = 0;
		simulation->philos[i].fork_r = i;
		simulation->philos[i].fork_l = (i + 1) % simulation->philos_num;
		simulation->philos[i].simulation = simulation;
	}
}

static int	init_mutex(t_program *simulation)
{
	int	i;

	i = -1;
	while (++i < simulation->philos_num)
	{
		if (pthread_mutex_init(&simulation->forks[i], NULL))
		{
			while (--i < 0)
				pthread_mutex_destroy(&simulation->forks[i]);
			return (printf("Error\nMutex init failed!\n"), 1);
		}
	}
	simulation->mut |= FORKS;
	if (pthread_mutex_init(&simulation->log_lock, NULL))
		return (printf("Error\nMutex init failed!\n"), 1);
	simulation->mut |= LOG;
	if (pthread_mutex_init(&simulation->meal_lock, NULL))
		return (printf("Error\nMutex init failed!\n"), 1);
	simulation->mut |= MEAL;
	return (0);
}

static void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->simulation->philos_num == 1)
		return (alone_philo(philo), NULL);
	if (philo->id % 2 == 0)
		usleep(200);
	philo_routine(philo);
	return (NULL);
}
