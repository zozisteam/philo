/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_check_fin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 15:07:36 by alalmazr          #+#    #+#             */
/*   Updated: 2022/08/17 15:24:20 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	done_dining_solo(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->dining->meals_m);
	if (philo->dining->all_ate && philo->dining->must_eat != -1)
		return (1);
	pthread_mutex_unlock(&philo->dining->meals_m);
	return (0);
}

int	check_death_solo(t_philosopher *philo)
{
	if (philo->dining->dead || philo->dining->all_ate)
		return (1);
	if ((time_in_ms() - philo->last_meal) > philo->dining->tt_die)
	{
		pthread_mutex_lock(&philo->dining->dead_m);
		philo->dining->dead = 1;
		pthread_mutex_unlock(&philo->dining->dead_m);
		usleep(100);
		return (1);
	}
	return (0);
}

void	check_finish_all_background(t_philosopher *philo, t_dining *dining)
{
	int	i;

	i = 0;
	while (1)
	{
		if ((time_in_ms() - philo[i].last_meal) > philo->dining->tt_die && dining->all_ate == 0)
		{
			usleep(100);
			pthread_mutex_lock(&dining->print_mutex);
			printf("%lldms %d died\n", (time_in_ms() - philo->dining->start_time), philo[i].id + 1);
			dining->dead = 1;
			pthread_mutex_unlock(&dining->print_mutex);
			return ;
		}
		if(dining->must_eat > -1)
		{
			while (i < dining->no_of_philo)
			{
				if (philo[i].c_ate < philo[i].dining->must_eat)
					break ;
				i++;
			}
			if (i == dining->no_of_philo)
			{
				dining->all_ate = 1;
				return ;
			}
		}
		i = (i + 1) % dining->no_of_philo;
		usleep(200);
	}
	return ;
}