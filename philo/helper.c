/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 21:28:18 by yslami            #+#    #+#             */
/*   Updated: 2025/04/20 17:40:46 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_logs(t_philo *philo, char *str)
{
	int	status;

	pthread_mutex_lock(&philo->simulation->died_lock);
	status = philo->simulation->stop_flag;
	pthread_mutex_unlock(&philo->simulation->died_lock);
	if (status != DEAD && status != STOP)
	{
		pthread_mutex_lock(&philo->simulation->log_lock);
		printf("%ld %d %s\n", get_time() - philo->simulation->start_time, \
		philo->id, str);
		pthread_mutex_unlock(&philo->simulation->log_lock);
	}
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
	if (simulation->mut & DIED)
		pthread_mutex_destroy(&simulation->died_lock);
	if (simulation->forks)
		free(simulation->forks);
	if (simulation->philos)
		free(simulation->philos);
}

void	cleanup_threads(t_program *simulation, int i)
{
	while (--i > 0)
		pthread_join(simulation->philos[i].thread, NULL);
	printf("Error\nThread creation failed!\n");
}
