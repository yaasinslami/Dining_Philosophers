/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:41:35 by yslami            #+#    #+#             */
/*   Updated: 2025/02/25 17:07:08 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int ac, char **av)
{
	t_program	*simulation;

	simulation = malloc(sizeof(t_program));
	if (!simulation)
		return (printf("Error\nMalloc failed!\n"), 1);
	if (parsing(ac, av, simulation))
		return (free(simulation), 1);
	// if (simulation && !start_simulation(simulation))
		// return (kill_world(&simulation), 1);
	start_simulation(simulation);
	sem_wait(simulation->stop_flag);
	kill_world(&simulation);
	return (0);
}
