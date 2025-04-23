/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:04:06 by yslami            #+#    #+#             */
/*   Updated: 2025/04/21 14:17:16 by yslami           ###   ########.fr       */
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

void	ft_sleep(time_t time)
{
	time_t	now_time;

	now_time = get_time();
	while (get_time() - now_time < time)
		usleep(200);
}
