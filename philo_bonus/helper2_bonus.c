/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:04:06 by yslami            #+#    #+#             */
/*   Updated: 2025/04/14 17:16:29 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

int	child_sem(t_philo *philo)
{
	char	*waiter_sem;

	waiter_sem = ft_strjoin("philo", ft_itoa(philo->id));
	if (!waiter_sem)
		return (printf("Malloc failed!\n"), exit(2), 0);
	sem_unlink(waiter_sem);
	philo->simulation->child_sem = sem_open(waiter_sem, O_CREAT | O_EXCL, \
		0644, 1);
	free(waiter_sem);
	if (philo->simulation->child_sem == SEM_FAILED)
		return (printf("Semaphore initialization failed!\n"), exit(2), 0);
	return (1);
}
