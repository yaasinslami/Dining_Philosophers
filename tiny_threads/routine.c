/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:36:42 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 22:28:31 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	alone_philo(t_philo *philo);
static int	eat(t_philo *philo);
static void	philo_hang(time_t time);
static void	philo_routine(t_philo *philo);

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->simulation->philos_num == 1)
		return (alone_philo(philo), NULL);
	if (philo->id % 2)
		philo_hang(philo->simulation->time_to_eat / 2);
	philo_routine(philo);
	return (NULL);
}

static void	philo_routine(t_philo *philo)
{
	while (!should_stop(philo->simulation))
	{
		take_forks(philo);
		if (!eat(philo))
			break ;
		print_logs(philo, "is sleeping");
		philo_hang(philo->simulation->time_to_sleep);
		print_logs(philo, "is thinking");
	}
}

static void	philo_hang(time_t time)
{
	time_t	now_time;

	now_time = get_time();
	while (get_time() - now_time < time)
		usleep(200);
}

static int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->simulation->mealtime_lock);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->simulation->mealtime_lock);
	print_logs(philo, "is eating");
	philo_hang(philo->simulation->time_to_eat);
	if (should_stop(philo->simulation))
		return (putdown_forks(philo), 0);
	putdown_forks(philo);
	pthread_mutex_lock(&philo->simulation->meal_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->simulation->meal_lock);
	if (should_stop(philo->simulation))
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
