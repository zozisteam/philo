/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_check_fin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 15:07:36 by alalmazr          #+#    #+#             */
/*   Updated: 2022/09/21 16:14:06 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_ate(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->dining->meals_m);
	if (philo->dining->all_ate && philo->dining->must_eat != -1)
	{
		pthread_mutex_unlock(&philo->dining->meals_m);
		return (1);
	}
	pthread_mutex_unlock(&philo->dining->meals_m);
	return (0);
}

int	check_death_solo(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->dining->dead_m);
	if (philo->dining->dead || check_ate(philo))
	{
		pthread_mutex_unlock(&philo->dining->dead_m);
		return (1);
	}
	pthread_mutex_unlock(&philo->dining->dead_m);
	if ((time_in_ms() - philo->last_meal) > philo->dining->tt_die)
	{
		pthread_mutex_lock(&philo->dining->dead_m);
		philo->dining->dead = 1;
		pthread_mutex_unlock(&philo->dining->dead_m);
		return (1);
	}
	return (0);
}

int	bg_death(t_philosopher *philo, t_dining *dining, int i)
{
	pthread_mutex_lock(&dining->meals_m);
	if ((time_in_ms() - philo[i].last_meal) > philo->dining->tt_die
		&& dining->all_ate == 0)
	{
		pthread_mutex_unlock(&dining->meals_m);
		usleep(100);
		pthread_mutex_lock(&dining->print_mutex);
		pthread_mutex_lock(&dining->dead_m);
		printf("%lldms %d died\n",
			(time_in_ms() - philo->dining->start), philo[i].id);
		dining->dead = 1;
		pthread_mutex_unlock(&dining->dead_m);
		pthread_mutex_unlock(&dining->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&dining->meals_m);
	return (0);
}

//needed to compress check_finish_bg
int	eat_count(t_dining *dining, t_philosopher *philo)
{
	int	i;

	i = 0;
	while (i < dining->no_of_philo)
	{
		pthread_mutex_lock(&dining->meals_m);
		if (philo[i].c_ate < philo[i].dining->must_eat)
		{
			pthread_mutex_unlock(&dining->meals_m);
			break ;
		}
		pthread_mutex_unlock(&dining->meals_m);
		i++;
	}
	return (i);
}

void	check_finish_bg(t_philosopher *philo, t_dining *dining)
{
	int	i;

	i = 0;
	while (1)
	{
		if (bg_death(philo, dining, i))
			return ;
		if (dining->must_eat > -1)
		{
			if (eat_count(dining, philo) == dining->no_of_philo)
			{
				pthread_mutex_lock(&dining->meals_m);
				dining->all_ate = 1;
				pthread_mutex_unlock(&dining->meals_m);
				return ;
			}
		}
		i = (i + 1) % dining->no_of_philo;
		if (bg_death(philo, dining, i))
			return ;
		usleep(100);
	}
	return ;
}
