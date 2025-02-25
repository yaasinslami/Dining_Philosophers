/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:04:06 by yslami            #+#    #+#             */
/*   Updated: 2025/02/25 16:57:49 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
static size_t	nbrlen(long long n);
static void		make_itoa(char **res, long long nb, size_t len, size_t neg);

static size_t	nbrlen(long long n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		n *= -1;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	make_itoa(char **res, long long nb, size_t len, size_t neg)
{
	if (!res || !*res)
		return ;
	if (nb < 0)
	{
		neg = 1;
		nb *= -1;
		(*res)[0] = '-';
	}
	(*res)[len] = '\0';
	while (len > neg)
	{
		(*res)[len - 1] = (nb % 10) + '0';
		nb /= 10;
		len--;
	}
}

char	*ft_itoa(int num)
{
	long		nb;
	size_t		len;
	size_t		neg;
	char		*res;

	neg = 0;
	nb = num;
	len = nbrlen(nb);
	if (nb < 0)
		len += 1;
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	make_itoa(&res, nb, len, neg);
	return (res);
}
*/

static size_t	intlen(long long n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		n *= -1;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*putitoa(size_t neg, size_t len, char *str, long long nb)
{
	if (nb < 0)
	{
		neg = 1;
		nb *= -1;
		str[0] = '-';
	}
	str[len] = '\0';
	while (len > neg)
	{
		str[len - 1] = (nb % 10) + 48;
		nb /= 10;
		len--;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	long long	nb;
	size_t		len;
	size_t		neg;
	char		*str;

	neg = 0;
	nb = n;
	len = intlen(nb);
	if (nb < 0)
		len += 1;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	putitoa(neg, len, str, nb);
	return (str);
}

void	ft_hang(int time)
{
	time_t	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(200);
}

void	kill_world(t_program **simulation)
{
	int	i;

	i = 0;
	while ((*simulation) && i < (*simulation)->philos_num)
		kill((*simulation)->philos[i++].philo_pid, SIGKILL);
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	sem_unlink(STOP_SEM);
	sem_close((*simulation)->forks);
	sem_close((*simulation)->log_sem);
	sem_close((*simulation)->stop_flag);
	if (simulation && *simulation && (*simulation)->philos)
		free((*simulation)->philos);
	if (simulation && *simulation)
		free(*simulation);
	exit(0);
}
