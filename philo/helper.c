/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 21:28:18 by yslami            #+#    #+#             */
/*   Updated: 2025/04/17 11:45:47 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_logs(t_philo *philo, char *str)
{
	int	should_continue;

	pthread_mutex_lock(&philo->simulation->meal_lock);
	should_continue = philo->simulation->stop_flag != STOP;
	pthread_mutex_unlock(&philo->simulation->meal_lock);
	if (should_continue)
	{
		pthread_mutex_lock(&philo->simulation->log_lock);
		printf("%ld %d %s\n", get_time() - philo->simulation->start_time, \
		philo->id, str);
		pthread_mutex_unlock(&philo->simulation->log_lock);
	}
}

void	unlock_mutexes(t_philo *philo, int state)
{
	if (state == EAT)
	{
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_l]);
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_r]);
	}
}

void	free_simulation(t_program *simulation)
{
	int	i;

	if (!simulation)
		return ;
	i = -1;
	if (simulation->mut & FORKS)
	{
		while (simulation->forks && ++i < simulation->philos_num)
			pthread_mutex_destroy(&simulation->forks[i]);
	}
	if (simulation->mut & LOG)
		pthread_mutex_destroy(&simulation->log_lock);
	if (simulation->mut & MEAL)
		pthread_mutex_destroy(&simulation->meal_lock);
	if (simulation->forks)
		free(simulation->forks);
	if (simulation->philos)
		free(simulation->philos);
}

int	check_number_of_eats(t_philo *philo)
{
	pthread_mutex_lock(&philo->simulation->meal_lock);
	if (philo->simulation->number_of_eats != -1 && \
		philo->meals_eaten >= philo->simulation->number_of_eats)
	{
		pthread_mutex_unlock(&philo->simulation->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->simulation->meal_lock);
	return (0);
}

time_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		write(2, "gettimeofday() Error\n", 22);
		return (-1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
