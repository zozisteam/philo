/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_dining.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:29:37 by alalmazr          #+#    #+#             */
/*   Updated: 2022/08/17 16:35:35 by alalmazr         ###   ########.fr       */
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

int	eat(t_dining *dining, t_philosopher *philo)
{
	if (check_death_solo(philo))
		return (1);
	pthread_mutex_lock(&dining->forks_mutex[philo->l_fork_id]);
	print(philo, (time_in_ms() - philo->dining->start_time), "has taken a fork");
	if (check_death_solo(philo))
	{
		pthread_mutex_unlock(&dining->forks_mutex[philo->l_fork_id]);
		return (1);
	}
	pthread_mutex_lock(&dining->forks_mutex[philo->r_fork_id]);
	print(philo, (time_in_ms() - philo->dining->start_time), "has taken a fork");
	if (check_death_solo(philo))
	{
		pthread_mutex_unlock(&dining->forks_mutex[philo->l_fork_id]);
		pthread_mutex_unlock(&dining->forks_mutex[philo->r_fork_id]);	
		return (1);
	}
	print(philo, (time_in_ms() - philo->dining->start_time), "is eating");
	pthread_mutex_lock(&dining->meals_m);
	philo->last_meal = time_in_ms();
	philo->c_ate++;
	pthread_mutex_unlock(&dining->meals_m);
	philo_idle(philo, dining->tt_eat);
	pthread_mutex_unlock(&dining->forks_mutex[philo->l_fork_id]);
	pthread_mutex_unlock(&dining->forks_mutex[philo->r_fork_id]);
	return (0);
}

void	*dine(void *phil)
{
	t_philosopher	*philo;
	t_dining		*dining;

	philo = (t_philosopher *)phil;
	dining = philo->dining;
	if (enter_philo(dining, philo))
		return (0);
	philo->last_meal = time_in_ms();
	while (dining->dead == 0 && dining->all_ate == 0)
	{
		if (eat(dining, philo))
			return (0);
		if (check_death_solo(philo) || done_dining_solo(philo))
		 	return (0);
		print(philo, (time_in_ms() - philo->dining->start_time), "is sleeping");
		philo_idle(philo, dining->tt_sleep);
		print(philo, (time_in_ms() - philo->dining->start_time), "is thinking");
		if (check_death_solo(philo) || done_dining_solo(philo))
			return (0);
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

		if (pthread_create(&philosophers[i].thread_id, NULL, dine, &philosophers[i]))
			return (1);
		usleep(50);
		i++;
	}
	check_finish_all_background(philosophers, dining);
	/* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */
	//protecting threads  //waiting for children processes to finish before parent process ends found out it works without
	// i = 0;
	// while (i < dining->no_of_philo)
	// {
	// 	if (pthread_join(philosophers[i].thread_id, NULL))
	// 		return (1);
	// 	printf("pthread_join philo [%d]\n",i+1);
	// 	i++;
	// }
	return (0);
}