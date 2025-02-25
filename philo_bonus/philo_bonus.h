/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 20:29:28 by yslami            #+#    #+#             */
/*   Updated: 2025/02/25 15:00:38 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdlib.h>
# include <fcntl.h>
# include <semaphore.h>
# include <stdio.h>
# include <errno.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
#include <signal.h>
#include <sys/time.h>

# define MAX_PHILOS 200
# define STARVE 1
# define ERROR 2
# define STOP 3

# define FORK_SEM "/fork_sem"
# define LOG_SEM "/log_sem"
# define STOP_SEM "/dead_sem"

typedef struct s_philo
{
	pthread_t			monitor;
	pthread_t			waiter;
	pid_t				philo_pid;
	int					id;
	int					meals_eaten;
	time_t				last_meal_time;
	struct s_program	*simulation;
}	t_philo;

typedef struct s_program
{
	int					philos_num;
	int					number_of_eats;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	// int					dead_philo;
	time_t				start_time;
	// time_t				death_time;
	sem_t				*forks;
	sem_t				*log_sem;
	sem_t				*stop_flag;
	sem_t				*child_sem;
	t_philo				*philos;
}	t_program;

// from mandatory
int		parsing(int ac, char **av, t_program *simulation);
int		start_simulation(t_program *simulation);
void	*monitor(void *arg);
void	*waiter(void *arg);
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	ft_sleep(t_philo *philo);
void	ft_hang(int time);
void	kill_world(t_program **simulation);
void	print_logs(t_philo *philo, char *str);
time_t	get_time(void);
size_t	ft_strlen(char *s);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_itoa(int num);

int	wait_philos(t_program *simulation);
// int find_philo_by_pid(t_program *simulation, pid_t pid);

#endif