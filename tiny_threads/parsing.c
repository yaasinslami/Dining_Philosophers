/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:59:15 by yslami            #+#    #+#             */
/*   Updated: 2025/02/23 09:35:57 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	fill_data(t_program *simulation, int num, int i);
static int	ft_atoi(char *str);
static int	ft_handle_overflow_int(unsigned long long nbr, int ndigit);

int	parsing(int ac, char **av, t_program *simulation)
{
	int		i;
	int		num;

	if (ac != 5 && ac != 6)
		return (printf("Error\nInvalid arguments!\n"), 1);
	i = 0;
	simulation->number_of_eats = -1;
	while (++i < ac)
	{
		num = ft_atoi(av[i]);
		if (num == -1)
			return (printf("Error\nOverflow detected!\n"), 1);
		if (num == -2)
			return (printf("Error\nOnly digits are accepted!\n"), 1);
		if (i == 1 && (num == 0 || num > MAX_PHILOS))
			return (printf("Error\nInvalid philos number!\n"), 1);
		if ((i > 1 && i < 5) && num < 60)
			return (printf("Error\nUnrealistic time simulation!\n"), 1);
		fill_data(simulation, num, i);
	}
	return (0);
}

static void	fill_data(t_program *simulation, int num, int i)
{
	if (i == 1)
		simulation->philos_num = num;
	else if (i == 2)
		simulation->time_to_die = num;
	else if (i == 3)
		simulation->time_to_eat = num;
	else if (i == 4)
		simulation->time_to_sleep = num;
	else if (i == 5 && num > 0)
		simulation->number_of_eats = num;
}

static int	ft_atoi(char *str)
{
	unsigned long long	result;
	int					overflow;
	int					i;

	i = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] == '+')
		i++;
	result = 0;
	while (str[i] == '0')
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		overflow = ft_handle_overflow_int(result, str[i] - '0');
		if (overflow == -1)
			return (-1);
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	if ((str[i] && (str[i] < '0' || str[i] > '9')) || \
		(!str[i] && !result))
		return (-2);
	return ((int)(result));
}

static int	ft_handle_overflow_int(unsigned long long nbr, int ndigit)
{
	if ((nbr > INT_MAX / 10)
		|| (nbr == INT_MAX / 10 && ndigit > INT_MAX % 10))
		return (-1);
	return (1);
}
