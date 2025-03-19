/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 20:29:28 by yslami            #+#    #+#             */
/*   Updated: 2025/03/18 16:50:56 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

#include <sys/wait.h>
# include <sys/types.h>
# include <stdlib.h>
# include <semaphore.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <signal.h>
# include <sys/time.h>

# define MAX_PHILOS 200

# define FORK_SEM "/fork_sem"
# define LOG_SEM "/log_sem"

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
	time_t				start_time;
	sem_t				*forks;
	sem_t				*log_sem;
	sem_t				*child_sem;
	sem_t				*ready_sem;
	sem_t				*start_sem;
	t_philo				*philos;
}	t_program;

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
int		wait_philos(t_program *simulation);

#endif
