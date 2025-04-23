/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:37:05 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 12:50:04 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks_mutex(t_program *simulation)
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
	return (0);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->simulation->forks[philo->fork_r]);
		print_logs(philo, "has taken a fork");
		pthread_mutex_lock(&philo->simulation->forks[philo->fork_l]);
		print_logs(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->simulation->forks[philo->fork_l]);
		print_logs(philo, "has taken a fork");
		pthread_mutex_lock(&philo->simulation->forks[philo->fork_r]);
		print_logs(philo, "has taken a fork");
	}
}

void	putdown_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_l]);
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_r]);
	}
	else
	{
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_r]);
		pthread_mutex_unlock(&philo->simulation->forks[philo->fork_l]);
	}
}
