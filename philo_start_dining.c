/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_dining.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:29:37 by alalmazr          #+#    #+#             */
/*   Updated: 2022/09/21 13:34:45 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	enter_philo(t_dining *dining, t_philosopher *philo)
{
	if (philo->id % 2 == 1)
		usleep(50);
	if (dining->no_of_philo == 1)
	{
		while (1)
			if (check_death_solo(philo) == 1)
				return (1);
	}
	return (0);
}

void	fork_lock_order(t_dining *dining, t_philosopher *philo, int lock)
{
	if (lock == 1 && philo->id % 2 == 0)
	{
		pthread_mutex_lock(&dining->forks_mutex[philo->l_fork_id]);
		pthread_mutex_lock(&dining->forks_mutex[philo->r_fork_id]);
	}
	else if (lock == 1 && philo->id % 2 == 1)
	{
		pthread_mutex_lock(&dining->forks_mutex[philo->r_fork_id]);
		pthread_mutex_lock(&dining->forks_mutex[philo->l_fork_id]);
	}
	if (lock == 0 && philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&dining->forks_mutex[philo->l_fork_id]);
		pthread_mutex_unlock(&dining->forks_mutex[philo->r_fork_id]);
	}
	else if (lock == 0 && philo->id % 2 == 1)
	{
		pthread_mutex_unlock(&dining->forks_mutex[philo->r_fork_id]);
		pthread_mutex_unlock(&dining->forks_mutex[philo->l_fork_id]);
	}
	return ;
}

int	take_forks(t_dining *dining, t_philosopher *philo)
{
	fork_lock_order(dining, philo, 1);
	if (dining->forks[philo->l_fork_id] != philo->id
		&& dining->forks[philo->r_fork_id] != philo->id)
	{
		dining->forks[philo->l_fork_id] = philo->id;
		dining->forks[philo->r_fork_id] = philo->id;
		print(philo, (time_in_ms() - philo->dining->start), "has taken a fork");
		print(philo, (time_in_ms() - philo->dining->start), "has taken a fork");
		print(philo, (time_in_ms() - philo->dining->start), "is eating");
		philo_idle(philo, dining->tt_eat);
		pthread_mutex_lock(&dining->meals_m);
		philo->c_ate++;
		philo->last_meal = time_in_ms();
		pthread_mutex_unlock(&dining->meals_m);
		fork_lock_order(dining, philo, 0);
		return (0);
	}
	fork_lock_order(dining, philo, 0);
	return (0);
}

/* Wait till threads are complete before main continues. Unless we  */
/* wait we run the risk of executing an exit which will terminate   */
/* the process and all threads before the threads have completed.   */
//protecting threads
//waiting for children processes to finish 
//before parent process ends found out it works without
int	join(t_dining *dining)
{
	t_philosopher	*philosophers;
	int				i;

	philosophers = dining->philos;
	i = 0;
	while (i < dining->no_of_philo)
	{
		if (pthread_join(philosophers[i].thread_id, NULL))
			return (1);
		i++;
	}
	return (0);
}

int	start(t_dining *dining)
{
	t_philosopher	*philosophers;
	int				i;

	philosophers = dining->philos;
	i = 0;
	while (i < dining->no_of_philo)
	{
		if (pthread_create(&philosophers[i].thread_id,
				NULL, dine, &philosophers[i]))
			return (1);
		usleep(50);
		i++;
	}
	check_finish_bg(philosophers, dining);
	if (join(dining))
		return (1);
	return (0);
}
