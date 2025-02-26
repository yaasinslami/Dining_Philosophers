/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 21:19:59 by yslami            #+#    #+#             */
/*   Updated: 2025/02/26 22:28:01 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

# define MAX_PHILOS 200
# define STOP 1
# define EAT 2
# define SLEEP 3

typedef struct s_philo
{
	pthread_t			thread;
	int					fork_r;
	int					fork_l;
	int					meals_eaten;
	int					id;
	time_t				last_meal_time;
	struct s_program	*simulation;
}	t_philo;

typedef struct s_program
{
	int					philos_num;
	int					number_of_eats;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					stop_flag;
	int					dead_philo;
	time_t				start_time;
	time_t				death_time;
	pthread_mutex_t		*forks;
	pthread_mutex_t		log_lock;
	pthread_mutex_t		meal_lock;
	t_philo				*philos;
}	t_program;

int		parsing(int ac, char **av, t_program *simulation);
int		start_simulation(t_program *simulation);
void	print_logs(t_philo *philo, char *str);
void	alone_philo(t_philo *philo);
void	philo_routine(t_philo *philo);
void	unlock_mutexes(t_philo *philo, int state);
int		check_number_of_eats(t_philo *philo);
void	free_simulation(t_program *simulation);
time_t	get_time(void);

#endif