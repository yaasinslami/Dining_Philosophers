/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 21:28:18 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 22:30:14 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_logs(t_philo *philo, char *str)
{
	if (!should_stop(philo->simulation))
	{
		pthread_mutex_lock(&philo->simulation->log_lock);
		printf("%ld %d %s\n", get_time() - philo->simulation->start_time, \
		philo->id, str);
		pthread_mutex_unlock(&philo->simulation->log_lock);
	}
}

int	check_number_of_eats(t_philo *philo)
{
	int	meals_eaten;

	pthread_mutex_lock(&philo->simulation->meal_lock);
	meals_eaten = philo->meals_eaten;
	pthread_mutex_unlock(&philo->simulation->meal_lock);
	if (philo->simulation->number_of_eats != -1 && \
		meals_eaten >= philo->simulation->number_of_eats)
		return (1);
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
	if (simulation->mut & TIME)
		pthread_mutex_destroy(&simulation->mealtime_lock);
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
