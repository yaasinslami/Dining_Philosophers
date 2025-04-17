/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 20:29:28 by yslami            #+#    #+#             */
/*   Updated: 2025/04/17 16:16:59 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdlib.h>
# include <semaphore.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>
# include <signal.h>

# define MAX_PHILOS 200
# define PHILOS 1

# define FORK_SEM "/fork_sem"
# define LOG_SEM "/log_sem"

typedef struct s_philo
{
	pthread_t			monitor;
	pid_t				philo_pid;
	int					id;
	int					meals_eaten;
	time_t				last_meal_time;
	time_t				next_meal;
	sem_t				*meal_check;
	struct s_program	*simulation;
}	t_philo;

typedef struct s_program
{
	int					info;
	int					philos_num;
	int					number_of_eats;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	time_t				start_time;
	sem_t				*forks;
	sem_t				*log_sem;
	sem_t				*child_sem;
	t_philo				*philos;
}	t_program;

int		parsing(int ac, char **av, t_program *simulation);
int		start_simulation(t_program *simulation);
void	*monitor(void *arg);
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	ft_sleep(t_philo *philo);
void	kill_world(t_program **simulation);
void	print_logs(t_philo *philo, char *str, int id);
time_t	get_time(void);
int		check_number_of_eats(t_philo *philo);
int		child_sem(t_philo *philo);
void	wait_philos(t_program *simulation);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_itoa(int num);
void	kill_forked(t_program *simulation, int last);

#endif
