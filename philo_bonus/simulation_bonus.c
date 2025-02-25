/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:53:40 by yslami            #+#    #+#             */
/*   Updated: 2025/02/24 20:51:58 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	start_simulation(t_program *simulation)
{
	if (!initialize_semaphores(simulation))
		return (perror("Semaphore initialization failed!"), 0);
	if (!initialize_philos(simulation))
		return (perror("Philosophers initialization failed!"), 0);
}

int	initialize_semaphores(t_program *simulation)
{
	unlink(FORK_SEM);
	simulation->forks = sem_open(FORK_SEM, O_CREAT | O_EXCL, 0644, \
		simulation->philos_num);
	unlink(LOG_SEM);
	simulation->log_sem = sem_open(LOG_SEM, O_CREAT | O_EXCL, 0644, 1);
	unlink(DEAD_SEM);
	simulation->philo_dead = sem_open(DEAD_SEM, O_CREAT | O_EXCL, 0644, 0);
	if (simulation->forks == SEM_FAILED || simulation->log_sem == SEM_FAILED || \
		simulation->philo_dead == SEM_FAILED)
		return (0);
	return (1);
}

int	initialize_philos(t_program *simulation)
{
	int	i;

	i = -1;
	simulation->philos = malloc(sizeof(t_philo) * simulation->philos_num);
	if (!simulation->philos)
		return (perror("Malloc failed!"), 0);
	while (++i < simulation->philos_num)
	{
		simulation->philos[i].id = i + 1;
		simulation->philos[i].meals_eaten = 0;
		simulation->philos[i].simulation = simulation;
		simulation->philos[i].last_meal_time = get_time();
		if (!forkeach_philo(&simulation->philos[i]))
			return (0);
	}
	return (wait_philos(simulation));
}

int	fork_each_philo(t_philo *philo)
{
	philo->philo_pid = fork();
	if (philo->philo_pid == -1)
		return (perror("Fork failed!"), 0);
	if (philo->philo_pid == 0 && !child_philo(philo))
		return (0);
	return (1);
}

void	monitor(void *arg)
{
	t_philo	*philo;
	time_t	time_without_eat;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->child_sem);
		time_without_eat = get_time() - philo->last_meal_time;
		if (time_without_eat > philo->simulation->time_to_die)
		{
			print_logs(philo, "died");
			sem_post(philo->simulation->philo_dead);
			return (NULL);
		}
		sem_post(philo->child_sem);
		usleep(200);
	}
}

int	child_philo(t_philo *philo)
{
	char	*waiter_sem;

	waiter_sem = ft_strjoin("philo_", ft_itoa(philo->id));
	sem_unlink(waiter_sem);
	philo->child_sem = sem_open(waiter_sem, O_CREAT | O_EXCL, 0644, 1);
	free(waiter_sem);
	if (philo->child_sem == SEM_FAILED)
		return (perror("Semaphore initialization failed!"), 0);
	if (pthread_create(&philo->monitor, NULL, monitor, philo))
		return (perror("Thread creation failed!"), exit(2), 0);
	if (pthread_detach(philo->monitor))
		return (perror("Thread detach failed!"), 0);
	if (pthread_create(&philo->waiter, NULL, waiter, philo))
		return (perror("Thread creation failed!"), exit(2), 0);
}

