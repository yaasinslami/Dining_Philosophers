/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:16:55 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 16:17:46 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static sem_t	*create_sem(const char *prefix, int id)
{
	char	*id_str;
	char	*sem_name;
	sem_t	*sem;

	id_str = ft_itoa(id);
	if (!id_str)
		return (printf("Malloc failed!\n"), exit(2), NULL);
	sem_name = ft_strjoin((char *)prefix, id_str);
	free(id_str);
	if (!sem_name)
		return (printf("Malloc failed!\n"), exit(2), NULL);
	sem_unlink(sem_name);
	sem = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 1);
	sem_unlink(sem_name);
	free(sem_name);
	if (sem == SEM_FAILED)
		return (printf("sem_open failed\n"), exit(2), NULL);
	return (sem);
}

int	meal_sem(t_philo *philo)
{
	philo->meal_sem = create_sem(MEAL, philo->id);
	if (!philo->meal_sem)
		return (0);
	philo->mealtime_sem = create_sem(TIME, philo->id);
	if (!philo->mealtime_sem)
		return (0);
	return (1);
}
