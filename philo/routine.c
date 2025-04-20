/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:36:42 by yslami            #+#    #+#             */
/*   Updated: 2025/04/20 21:21:00 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	alone_philo(t_philo *philo);
static int	eat(t_philo *philo);
static int	philo_hang(t_philo *philo, time_t time);
static void	philo_routine(t_philo *philo);

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->simulation->philos_num == 1)
		return (alone_philo(philo), NULL);
	if (philo->id % 2)
		usleep((philo->simulation->time_to_eat / 2) * 1000);
	philo_routine(philo);
	return (NULL);
}

static void	philo_routine(t_philo *philo)
{
	while (1)
	{
		take_forks(philo);
		if (!eat(philo))
			break ;
		print_logs(philo, "is sleeping");
		if (!philo_hang(philo, philo->simulation->time_to_sleep))
			break ;
		print_logs(philo, "is thinking");
	}
}

static int	philo_hang(t_philo *philo, time_t time)
{
	time_t	now_time;

	now_time = get_time();
	while (get_time() - now_time < time)
		usleep(200);
	if (should_stop(philo->simulation))
		return (0);
	return (1);
}

static int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->simulation->meal_lock);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->simulation->meal_lock);
	print_logs(philo, "is eating");
	if (!philo_hang(philo, philo->simulation->time_to_eat))
		return (putdown_forks(philo), 0);
	putdown_forks(philo);
	if (check_number_of_eats(philo))
		return (0);
	return (1);
}

static void	alone_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->simulation->forks[philo->fork_r]);
	print_logs(philo, "has taken a fork");
	while (get_time() - philo->last_meal_time < philo->simulation->time_to_die)
		usleep(200);
	pthread_mutex_unlock(&philo->simulation->forks[philo->fork_r]);
}
