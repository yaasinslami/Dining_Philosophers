/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 21:21:18 by yslami            #+#    #+#             */
/*   Updated: 2025/02/26 23:21:34 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void check_leaks(void) {
    system("leaks philo");
}

int	main(int ac, char **av)
{
	// atexit(check_leaks);
	t_program	simulation;

	if (parsing(ac, av, &simulation))
		return (1);
	start_simulation(&simulation);
	free_simulation(&simulation);
	return (0);
}
