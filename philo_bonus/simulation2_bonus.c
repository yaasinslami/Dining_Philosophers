/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:53:28 by yslami            #+#    #+#             */
/*   Updated: 2025/02/25 17:09:47 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor(void *arg)
{
	t_philo	*philo;
	time_t	time_without_eat;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->simulation->child_sem);
		time_without_eat = get_time() - philo->last_meal_time;
		if (time_without_eat > philo->simulation->time_to_die && \
			philo->meals_eaten != philo->simulation->number_of_eats)
		{
			print_logs(philo, "died");
			sem_post(philo->simulation->stop_flag);
			// return (exit(1), NULL);
				// exit(1);
		}
		sem_post(philo->simulation->child_sem);
		// usleep(300);
	}
	return (NULL);
}

void	*waiter(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		take_forks(philo);
		eat(philo);
		// sem_wait(philo->simulation->child_sem);
		// if (philo->meals_eaten == philo->simulation->number_of_eats)
		// 	sem_post(philo->simulation->stop_flag);
			// exit(STOP);
		// sem_post(philo->simulation->child_sem);
		ft_sleep(philo);
		print_logs(philo, "is thinking");
	}
}

void	take_forks(t_philo *philo)
{
	sem_wait(philo->simulation->forks);
	print_logs(philo, "has taken a fork");
	sem_wait(philo->simulation->forks);
	print_logs(philo, "has taken a fork");
}

void	eat(t_philo *philo)
{
	print_logs(philo, "is eating");
	ft_hang(philo->simulation->time_to_eat);
	sem_wait(philo->simulation->child_sem);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	sem_post(philo->simulation->child_sem);
	sem_post(philo->simulation->forks);
	sem_post(philo->simulation->forks);
}

void	ft_sleep(t_philo *philo)
{
	print_logs(philo, "is sleeping");
	ft_hang(philo->simulation->time_to_sleep);
}
