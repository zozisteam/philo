/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:47:50 by alalmazr          #+#    #+#             */
/*   Updated: 2022/09/17 17:44:14 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000LL + time.tv_usec / 1000);
}

int	philo_idle(t_philosopher *philo, int tt_wait)
{
	long long	t;

	t = time_in_ms();
	while (time_in_ms() - t < tt_wait)
	{
		if (check_death_solo(philo) || check_ate(philo))
			return (1);
		usleep(100);
	}
	return (0);
}

int	destroy_mutex(t_dining	*dining)
{
	int	i;

	i = 0;
	while (i < dining->no_of_philo)
	{
		if (pthread_mutex_destroy(&(dining->forks_mutex[i])) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_destroy(&(dining->dead_m)) != 0)
		return (1);
	if (pthread_mutex_destroy(&(dining->meals_m)) != 0)
		return (1);
	if (pthread_mutex_destroy(&(dining->print_mutex)) != 0)
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int				i;
	unsigned long	integer;

	integer = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-')
		return (-1);
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
			integer = integer * 10 + (str[i] - '0');
		else
			return (-1);
		i++;
	}
	return ((int)integer);
}

void	print(t_philosopher *philo, unsigned long time, char *is_doing)
{
	if (check_death_solo(philo) == 0)
	{
		pthread_mutex_lock(&philo->dining->print_mutex);
		printf("%lums	%d %s\n", time, philo->id + 1, is_doing);
		pthread_mutex_unlock(&philo->dining->print_mutex);
	}
}
