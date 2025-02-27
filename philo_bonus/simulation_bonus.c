/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:53:40 by yslami            #+#    #+#             */
/*   Updated: 2025/02/27 10:26:26 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	initialize_semaphores(t_program *simulation);
static int	initialize_philos(t_program *simulation);
static int	forkeach_philo(t_philo *philo);
static int	child_philo(t_philo *philo);

int	start_simulation(t_program *simulation)
{
	if (!initialize_semaphores(simulation))
		return (printf("Semaphore initialization failed!(start_simulation \
			function)"), 0);
	if (!initialize_philos(simulation))
		return (printf("Philosophers initialization failed!"), 0);
	return (1);
}

static int	initialize_semaphores(t_program *simulation)
{
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	simulation->forks = sem_open(FORK_SEM, O_CREAT | O_EXCL, 0644, \
		simulation->philos_num);
	simulation->log_sem = sem_open(LOG_SEM, O_CREAT | O_EXCL, 0644, 1);
	if (simulation->forks == SEM_FAILED || simulation->log_sem == SEM_FAILED)
		return (0);
	return (1);
}

static int	initialize_philos(t_program *simulation)
{
	int	i;

	i = -1;
	simulation->philos = malloc(sizeof(t_philo) * simulation->philos_num);
	if (!simulation->philos)
		return (printf("Malloc failed!"), 0);
	simulation->start_time = get_time();
	while (++i < simulation->philos_num)
	{
		simulation->philos[i].id = i + 1;
		simulation->philos[i].meals_eaten = 0;
		simulation->philos[i].simulation = simulation;
		if (!forkeach_philo(&simulation->philos[i]))
			return (0);
	}
	return (wait_philos(simulation));
}

static int	forkeach_philo(t_philo *philo)
{
	philo->philo_pid = fork();
	if (philo->philo_pid == -1)
		return (printf("Fork failed!"), 0);
	if (philo->philo_pid == 0 && !child_philo(philo))
		exit(2);
	return (1);
}

static int	child_philo(t_philo *philo)
{
	char	*waiter_sem;

	waiter_sem = ft_strjoin("philo", ft_itoa(philo->id));
	if (!waiter_sem)
		return (printf("Malloc failed!"), 0);
	sem_unlink(waiter_sem);
	philo->simulation->child_sem = sem_open(waiter_sem, O_CREAT | O_EXCL, \
		0644, 1);
	free(waiter_sem);
	if (philo->simulation->child_sem == SEM_FAILED)
		return (printf("Semaphore initialization failed!"), exit(2), 0);
	if (pthread_create(&philo->monitor, NULL, monitor, philo))
		return (printf("Thread creation failed!"), exit(2), 0);
	if (pthread_detach(philo->monitor))
		return (printf("Thread detach failed!"), 0);
	sem_wait(philo->simulation->child_sem);
	philo->last_meal_time = get_time();
	sem_post(philo->simulation->child_sem);
	if (pthread_create(&philo->waiter, NULL, waiter, philo))
		return (printf("Thread creation failed!"), exit(2), 0);
	if (pthread_join(philo->waiter, NULL))
		return (printf("Thread join failed!"), exit(2), 0);
	return (1);
}
