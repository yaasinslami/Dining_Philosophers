/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:53:40 by yslami            #+#    #+#             */
/*   Updated: 2025/02/25 17:12:07 by yslami           ###   ########.fr       */
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
		return (perror("Semaphore initialization failed!(start_simulation function)"), 0);
	if (!initialize_philos(simulation))
		return (perror("Philosophers initialization failed!"), 0);
	return (1);
}

static int	initialize_semaphores(t_program *simulation)
{
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	sem_unlink(STOP_SEM);
	simulation->forks = sem_open(FORK_SEM, O_CREAT | O_EXCL, 0644, \
		simulation->philos_num);
	simulation->log_sem = sem_open(LOG_SEM, O_CREAT | O_EXCL, 0644, 1);
	simulation->stop_flag = sem_open(STOP_SEM, O_CREAT | O_EXCL, 0644, 0);
	if (simulation->forks == SEM_FAILED || simulation->log_sem == SEM_FAILED || \
		simulation->stop_flag == SEM_FAILED)
		return (0);
	return (1);
}

static int	initialize_philos(t_program *simulation)
{
	int	i;

	i = -1;
	simulation->philos = malloc(sizeof(t_philo) * simulation->philos_num);
	if (!simulation->philos)
		return (perror("Malloc failed!"), 0);
	simulation->start_time = get_time();
	while (++i < simulation->philos_num)
	{
		simulation->philos[i].id = i + 1;
		simulation->philos[i].meals_eaten = 0;
		simulation->philos[i].simulation = simulation;
		if (!forkeach_philo(&simulation->philos[i]))
			return (0);
	}
	// return (wait_philos(simulation), 1);
	return (1);
}

static int	forkeach_philo(t_philo *philo)
{
	philo->philo_pid = fork();
	if (philo->philo_pid == -1)
		return (perror("Fork failed!"), 0);
	if (philo->philo_pid == 0)
	{
		if (!child_philo(philo))
			return (0);
		exit(0);
	}
	return (1);
}

static int	child_philo(t_philo *philo)
{
	char	*waiter_sem;

	waiter_sem = ft_strjoin(ft_itoa(philo->id), "philo");
	if (!waiter_sem)
		return (perror("Malloc failed!"), 0);
	sem_unlink(waiter_sem);
	philo->simulation->child_sem = sem_open(waiter_sem, O_CREAT | O_EXCL, 0644, 1);
	free(waiter_sem);
	if (philo->simulation->child_sem == SEM_FAILED)
		return (perror("Semaphore initialization failed!"), 0);
	if (pthread_create(&philo->monitor, NULL, monitor, philo))
		return (perror("Thread creation failed!"), 0);
	if (pthread_detach(philo->monitor))
		return (perror("Thread detach failed!"), 0);
	sem_wait(philo->simulation->child_sem);
	philo->last_meal_time = get_time();
	sem_post(philo->simulation->child_sem);
	if (pthread_create(&philo->waiter, NULL, waiter, philo))
		return (perror("Thread creation failed!"), 0);
	if (pthread_join(philo->waiter, NULL))
		return (perror("Thread join failed!"), 0);
	return (1);
}

// int	wait_philos(t_program *simulation)
// {
// 	int	status;
// 	int	i;

// 	i = 0;
// 	while (waitpid(simulation->philos[i].philo_pid, &status, 0) != -1)
// 	{
// 		if (status == STARVE)
// 		{
// 			// print_logs(simulation->philos, "died");
// 			// sem_post(simulation->stop_flag);
// 			kill_world(&simulation);
// 			return (0);
// 		}
// 		if (i == simulation->philos_num)
// 			i = 0;
// 		i++;
// 	}
// 	kill_world(&simulation);
// 	// sem_post(simulation->stop_flag);
// 	return (1);
// }

// void	wait_philos(t_program *simulation)
// {
// 	int		status;
// 	pid_t	pid;
// 	int		remaining_philos;

// 	remaining_philos = simulation->philos_num;
// 	while (remaining_philos > 0)
// 	{
// 		pid = waitpid(-1, &status, 0); // Wait for any child process
// 		if (pid == -1)
// 			break; // No more processes to wait for

// 		if (WIFEXITED(status) && WEXITSTATUS(status) == STARVE)
// 		{
// 			int i = find_philo_by_pid(simulation, pid);
// 			if (i >= 0)
// 				print_logs(simulation->philos, "died");
// 			sem_post(simulation->stop_flag);
// 			break;
// 		}
// 		if (WIFEXITED(status) && WEXITSTATUS(status) == STOP)
// 			remaining_philos--;
// 	}
// 	sem_post(simulation->stop_flag);
// }

// int find_philo_by_pid(t_program *simulation, pid_t pid)
// {
// 	int i = 0;
// 	while (i < simulation->philos_num)
// 	{
// 		if (simulation->philos[i].philo_pid == pid)
// 			return i;
// 		i++;
// 	}
// 	return -1; // Not found
// }
