/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yslami <yslami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:59:57 by yslami            #+#    #+#             */
/*   Updated: 2025/04/17 16:13:50 by yslami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static size_t	ft_strlen(char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

static char	*ft_strdup(char *str)
{
	char	*res;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	j = 0;
	i = ft_strlen(str);
	res = (char *)malloc((i + 1) * sizeof(char));
	if (!res)
		return (NULL);
	while (j < i)
	{
		res[j] = str[j];
		j++;
	}
	res[j] = '\0';
	return (res);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		s2 = ft_strdup("");
	res = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (res == NULL)
		return (free(s2), NULL);
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (free(s2), res);
}

time_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		write(2, "gettimeofday() Error\n", 22);
		exit(2);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_logs(t_philo *philo, char *message, int id)
{
	sem_wait(philo->simulation->log_sem);
	printf("%ld %d %s\n", get_time() - philo->simulation->start_time, \
		philo->id, message);
	if (id == 0)
		sem_post(philo->simulation->log_sem);
}
