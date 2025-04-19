/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:36:42 by yslami            #+#    #+#             */
/*   Updated: 2025/04/20 00:14:07 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	eat(t_philo *philo);
static void	philo_hang(time_t time);

void	philo_routine(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->simulation->forks[philo->fork_l]);
		print_logs(philo, "has taken a fork");
		pthread_mutex_lock(&philo->simulation->forks[philo->fork_r]);
		print_logs(philo, "has taken a fork");
		if (eat(philo))
			break ;
		if (should_stop(philo->simulation))
			break ;
		print_logs(philo, "is sleeping");
		philo_hang(philo->simulation->time_to_sleep);
		if (should_stop(philo->simulation))
			break ;
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
	pthread_mutex_lock(&philo->simulation->meal_lock);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->simulation->meal_lock);
	print_logs(philo, "is eating");
	philo_hang(philo->simulation->time_to_eat);
	if (check_number_of_eats(philo))
	{
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_l]);
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_r]);
		return (1);
	}
	pthread_mutex_unlock(&philo->simulation->forks[philo->fork_l]);
	pthread_mutex_unlock(&philo->simulation->forks[philo->fork_r]);
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
