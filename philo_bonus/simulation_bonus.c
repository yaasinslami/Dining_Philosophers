/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:53:40 by yslami            #+#    #+#             */
/*   Updated: 2025/03/18 16:53:41 by yslami           ###   ########.fr       */
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
			function)\n"), 0);
	if (!initialize_philos(simulation))
		return (printf("Philosophers initialization failed!\n"), 0);
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
		return (printf("Malloc failed!\n"), 0);

	// Create synchronization semaphores
    sem_unlink("ready_sem");
    sem_unlink("start_sem");
    simulation->ready_sem = sem_open("ready_sem", O_CREAT | O_EXCL, 0644, 0);
    simulation->start_sem = sem_open("start_sem", O_CREAT | O_EXCL, 0644, 0);

    if (simulation->ready_sem == SEM_FAILED || simulation->start_sem == SEM_FAILED)
        return (printf("Semaphore initialization failed!\n"), 0);
	// simulation->start_time = get_time();
	while (++i < simulation->philos_num)
	{
		simulation->philos[i].id = i + 1;
		simulation->philos[i].meals_eaten = 0;
		simulation->philos[i].simulation = simulation;
		if (!forkeach_philo(&simulation->philos[i]))
			return (0);
	}
	 // Wait for all philosophers to be ready
    i = -1;
    while (++i < simulation->philos_num)
        sem_wait(simulation->ready_sem);

    // Set the start time and release all philosophers
    simulation->start_time = get_time();
    i = -1;
    while (++i < simulation->philos_num)
        sem_post(simulation->start_sem);
	return (wait_philos(simulation));
}

static int	forkeach_philo(t_philo *philo)
{
	philo->philo_pid = fork();
	if (philo->philo_pid == -1)
		return (printf("Fork failed!\n"), 0);
	if (philo->philo_pid == 0)
	{
		if (!child_philo(philo))
			exit(2);
	}
	return (1);
}

static int	child_philo(t_philo *philo)
{
	char	*waiter_sem;

	waiter_sem = ft_strjoin("philo", ft_itoa(philo->id));
	if (!waiter_sem)
		return (printf("Malloc failed!\n"), 0);
	sem_unlink(waiter_sem);
	philo->simulation->child_sem = sem_open(waiter_sem, O_CREAT | O_EXCL, \
		0644, 1);
	free(waiter_sem);
	if (philo->simulation->child_sem == SEM_FAILED)
		return (printf("Semaphore initialization failed!\n"), exit(2), 0);

	 // Signal that this philosopher is ready
    sem_post(philo->simulation->ready_sem);

    // Wait for the start signal from parent
    sem_wait(philo->simulation->start_sem);

    // Get the start time from shared memory
    philo->simulation->start_time = get_time();

	sem_wait(philo->simulation->child_sem);
	philo->last_meal_time = get_time();
	sem_post(philo->simulation->child_sem);
	if (pthread_create(&philo->monitor, NULL, monitor, philo))
		return (printf("Thread creation failed!\n"), exit(2), 0);
	if (pthread_detach(philo->monitor))
		return (printf("Thread detach failed!\n"), 0);
	if (pthread_create(&philo->waiter, NULL, waiter, philo))
		return (printf("Thread creation failed!\n"), exit(2), 0);
	if (pthread_join(philo->waiter, NULL))
		return (printf("Thread join failed!\n"), exit(2), 0);
	return (1);
}
