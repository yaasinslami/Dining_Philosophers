/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:53:28 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 16:31:49 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor(void *arg)
{
	t_philo	*philo;
	int		check;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->mealtime_sem);
		check = get_time() - philo->last_meal_time;
		sem_post(philo->mealtime_sem);
		if (check > philo->simulation->time_to_die)
		{
			print_logs(philo, "died", 1);
			exit(1);
		}
	}
	return (NULL);
}

void	take_forks(t_philo *philo)
{
	sem_wait(philo->simulation->forks);
	print_logs(philo, "has taken a fork", 0);
	sem_wait(philo->simulation->forks);
	print_logs(philo, "has taken a fork", 0);
}

void	eat(t_philo *philo)
{
	print_logs(philo, "is eating", 0);
	sem_wait(philo->mealtime_sem);
	philo->last_meal_time = get_time();
	sem_post(philo->mealtime_sem);
	ft_sleep(philo->simulation->time_to_eat);
	if (philo->simulation->number_of_eats != -1)
	{
		sem_wait(philo->meal_sem);
		philo->meals_eaten++;
		sem_post(philo->meal_sem);
	}
	sem_post(philo->simulation->forks);
	sem_post(philo->simulation->forks);
}

int	check_number_of_eats(t_philo *philo)
{
	int	meals_eaten;

	sem_wait(philo->meal_sem);
	meals_eaten = philo->meals_eaten;
	sem_post(philo->meal_sem);
	if (philo->simulation->number_of_eats != -1 && \
		meals_eaten >= philo->simulation->number_of_eats)
		return (1);
	return (0);
}
