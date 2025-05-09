/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:53:40 by yslami            #+#    #+#             */
/*   Updated: 2025/04/22 03:05:09 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	initialize_semaphores(t_program *simulation);
static int	initialize_philos(t_program *simulation);
static int	forkeach_philo(t_philo *philo);
static int	child_philo(t_philo *philo);

int	start_simulation(t_program *simulation)
{
	simulation->info = 0;
	if (!initialize_semaphores(simulation))
		return (printf("Semaphore initialization failed!\n"), 0);
	if (!initialize_philos(simulation))
		return (printf("Philosophers initialization failed!\n"), 0);
	return (1);
}

static int	initialize_semaphores(t_program *simulation)
{
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	sem_unlink(DONE_SEM);
	sem_unlink(END_SEM);
	simulation->forks = sem_open(FORK_SEM, O_CREAT | O_EXCL, 0644, \
		simulation->philos_num);
	simulation->log_sem = sem_open(LOG_SEM, O_CREAT | O_EXCL, 0644, 1);
	simulation->done_sem = sem_open(DONE_SEM, O_CREAT | O_EXCL, 0644, 0);
	simulation->end_sem = sem_open(END_SEM, O_CREAT | O_EXCL, 0644, 0);
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	sem_unlink(DONE_SEM);
	sem_unlink(END_SEM);
	if (simulation->forks == SEM_FAILED || simulation->log_sem == SEM_FAILED || \
		simulation->done_sem == SEM_FAILED)
		return (0);
	return (1);
}

static int	initialize_philos(t_program *simulation)
{
	int	i;

	done_monitor(simulation);
	simulation->philos = malloc(sizeof(t_philo) * simulation->philos_num);
	if (!simulation->philos)
		return (printf("Malloc failed!\n"), 0);
	simulation->info |= PHILOS;
	simulation->start_time = get_time();
	i = -1;
	while (++i < simulation->philos_num)
	{
		simulation->philos[i].id = i + 1;
		simulation->philos[i].meals_eaten = 0;
		simulation->philos[i].simulation = simulation;
		if (!forkeach_philo(&simulation->philos[i]))
			return (0);
	}
	wait_philos(simulation);
	return (1);
}

static int	forkeach_philo(t_philo *philo)
{
	philo->philo_pid = fork();
	if (philo->philo_pid == -1)
		return (printf("Fork failed!\n"), 0);
	if (philo->philo_pid == 0)
		child_philo(philo);
	return (1);
}

static int	child_philo(t_philo *philo)
{
	if (!meal_sem(philo))
		return (exit(2), 0);
	sem_wait(philo->mealtime_sem);
	philo->last_meal_time = get_time();
	sem_post(philo->mealtime_sem);
	if (pthread_create(&philo->monitor, NULL, monitor, philo))
		return (printf("Thread creation failed!\n"), exit(2), 0);
	if (pthread_detach(philo->monitor))
		return (printf("Thread detach failed!\n"), exit(2), 0);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		if (check_number_of_eats(philo))
			sem_post(philo->simulation->done_sem);
		print_logs(philo, "is sleeping", 0);
		ft_sleep(philo->simulation->time_to_sleep);
		print_logs(philo, "is thinking", 0);
	}
	return (1);
}
