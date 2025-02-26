/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:36:42 by yslami            #+#    #+#             */
/*   Updated: 2025/02/26 20:36:24 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	eat(t_philo *philo);
static int	philo_hang(t_philo *philo, time_t time, int state);

void	philo_routine(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->simulation->forks[philo->fork_r]);
		print_logs(philo, "has taken a fork");
		pthread_mutex_lock(&philo->simulation->forks[philo->fork_l]);
		print_logs(philo, "has taken a fork");
		if (eat(philo))
			break ;
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_l]);
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_r]);
		print_logs(philo, "is sleeping");
		if (philo_hang(philo, philo->simulation->time_to_sleep, SLEEP))
			break ;
		print_logs(philo, "is thinking");
		if (check_number_of_eats(philo))
			break ;
		usleep(100);
	}
}

static int	philo_hang(t_philo *philo, time_t time, int state)
{
	time_t	now_time;
	int		should_stop;

	now_time = get_time();
	while (get_time() - now_time < time)
	{
		pthread_mutex_lock(&philo->simulation->meal_lock);
		if (get_time() - philo->last_meal_time >= \
			philo->simulation->time_to_die && !philo->simulation->stop_flag)
		{
			philo->simulation->dead_philo = philo->id;
			philo->simulation->death_time = get_time() - \
				philo->simulation->start_time;
			philo->simulation->stop_flag = STOP;
		}
		should_stop = philo->simulation->stop_flag;
		pthread_mutex_unlock(&philo->simulation->meal_lock);
		if (should_stop)
			return (unlock_mutexes(philo, state), 1);
		usleep(100);
	}
	return (0);
}

static int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->simulation->meal_lock);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->simulation->meal_lock);
	print_logs(philo, "is eating");
	if (philo_hang(philo, philo->simulation->time_to_eat, EAT))
		return (1);
	return (0);
}

void	alone_philo(t_philo *philo)
{
	print_logs(philo, "has taken a fork");
	pthread_mutex_lock(&philo->simulation->forks[philo->fork_r]);
	while (get_time() - philo->last_meal_time < philo->simulation->time_to_die)
		usleep(500);
	philo->simulation->dead_philo = philo->id;
	philo->simulation->death_time = get_time() - philo->simulation->start_time;
	philo->simulation->stop_flag = STOP;
	pthread_mutex_unlock(&philo->simulation->forks[philo->fork_r]);
}
