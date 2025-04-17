/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:53:28 by yslami            #+#    #+#             */
/*   Updated: 2025/04/17 15:28:47 by yslami           ###   ########.fr       */
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
		sem_wait(philo->simulation->child_sem);
		check = get_time() - philo->next_meal;
		sem_post(philo->simulation->child_sem);
		if (check > 0)
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
	if (philo->simulation->number_of_eats != -1)
		philo->meals_eaten++;
	usleep(philo->simulation->time_to_eat * 1000);
	sem_wait(philo->simulation->child_sem);
	philo->last_meal_time = get_time();
	philo->next_meal = philo->last_meal_time + \
		philo->simulation->time_to_die;
	sem_post(philo->simulation->child_sem);
	sem_post(philo->simulation->forks);
	sem_post(philo->simulation->forks);
}

void	ft_sleep(t_philo *philo)
{
	print_logs(philo, "is sleeping", 0);
	usleep(philo->simulation->time_to_sleep * 1000);
}

int	check_number_of_eats(t_philo *philo)
{
	if (philo->simulation->number_of_eats != -1 && \
		philo->meals_eaten >= philo->simulation->number_of_eats)
		return (1);
	return (0);
}
