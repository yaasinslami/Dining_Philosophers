/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:37:05 by yslami            #+#    #+#             */
/*   Updated: 2025/04/20 19:47:39 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
