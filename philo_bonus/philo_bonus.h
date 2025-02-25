/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 20:29:28 by yslami            #+#    #+#             */
/*   Updated: 2025/02/24 18:38:10 by yslami           ###   ########.fr       */
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

# define MAX_PHILOS 200
# define STOP 1
# define EAT 2
# define SLEEP 3

# define FORK_SEM "/fork_sem"
# define LOG_SEM "/log_sem"
# define DEAD_SEM "/dead_sem"

typedef struct s_philo
{
	pthread_t			monitor;
	pthread_t			waiter;
	pid_t				philo_pid;
	int					id;
	int					meals_eaten;
	time_t				last_meal_time;
	sem_t				*child_sem;
	struct s_program	*simulation;
}	t_philo;

typedef struct s_program
{
	int					philos_num;
	int					number_of_eats;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int					stop_flag;
	int					dead_philo;
	time_t				start_time;
	time_t				death_time;
	sem_t				*forks;
	sem_t				*log_sem;
	sem_t				*philo_dead;
	t_philo				*philos;
}	t_program;

// from mandatory
int		parsing(int ac, char **av, t_program *simulation);
int		start_simulation(t_program *simulation);

void	print_logs(t_philo *philo, char *str);
void	alone_philo(t_philo *philo);
void	philo_routine(t_philo *philo);
void	unlock_mutexes(t_philo *philo, int state);
int		check_number_of_eats(t_philo *philo);
void	free_simulation(t_program *simulation);
time_t	get_time(void);
size_t	ft_strlen(char *s);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_itoa(int num);

#endif