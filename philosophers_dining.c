/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_dining.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 14:22:49 by alalmazr          #+#    #+#             */
/*   Updated: 2022/08/15 20:29:01 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	eat(t_dining *dining, t_philosopher *philo)
{
//	if ((philo->c_ate >= dining->must_eat) && dining->must_eat != -1)
//		return (0);
	if (check_death(philo))
		return (1);
	pthread_mutex_lock(&dining->forks_mutex[philo->l_fork_id]);
	print(philo, (time_in_ms() - philo->dining->start_time), "has taken a fork");
	pthread_mutex_lock(&dining->forks_mutex[philo->r_fork_id]);
	print(philo, (time_in_ms() - philo->dining->start_time), "has taken a fork");
	if (check_death(philo))
		return (1);
	print(philo, (time_in_ms() - philo->dining->start_time), "is eating");
	pthread_mutex_lock(&dining->meals_m);
	philo->last_meal = time_in_ms();
	philo->c_ate++;
	printf("philo id: %d\n philo c_ate: %d\n", philo->id, philo->c_ate);
	pthread_mutex_unlock(&dining->meals_m);
	if (philo->c_ate == philo->dining->must_eat) //change it
		philo->dining->all_ate++; //change it
	philo_sleep(philo, dining->tt_eat);
	pthread_mutex_unlock(&dining->forks_mutex[philo->l_fork_id]);
	pthread_mutex_unlock(&dining->forks_mutex[philo->r_fork_id]);
	return (0);
}

int	enter_philo(t_dining *dining, t_philosopher *philo)
{
	if (philo->id % 2 == 1)
		usleep(50);
	if (dining->no_of_philo == 1)
	{
		while (1)
			if (check_death(philo) == 1)
				return (1);
	}
	return (0);
}

void	*dine(void *phil)
{
	t_philosopher	*philo;
	t_dining		*dining;

	philo = (t_philosopher *)phil;
	dining = philo->dining;
	if (enter_philo(dining, philo)) //
		return (0);
	philo->last_meal = time_in_ms();
	while (dining->dead == 0 && dining->all_ate == 0)
	{
		if (eat(dining, philo))
			return (0);
		if (check_death(philo) || done_dining(philo))
			return (0);
		print(philo, (time_in_ms() - philo->dining->start_time), "is sleeping");
		philo_sleep(philo, dining->tt_sleep);
		print(philo, (time_in_ms() - philo->dining->start_time), "is thinking");
		if (check_death(philo) || done_dining(philo))
			return (0);
	}
	return (0);
}

int	philo_sleep(t_philosopher *philo, int sleep)
{
	long long	t;

	t = time_in_ms();
	while (time_in_ms() - t < sleep)
	{
		if (check_death(philo))
			return (1);
		usleep(100);
	}
	return (0);
}

//turn all_ate into a flag 
//
int	done_dining(t_philosopher *philo)
{
	 printf("must eat: %d\nphilo all ate: %d\n", philo->dining->must_eat, philo->dining->all_ate);
	if (philo->dining->must_eat <= philo->dining->all_ate && philo->dining->must_eat != -1)
	{
		printf("done eating\n");
		destroy_mutex(philo->dining);
		//free_param(philo, philosophers->forks_mutex[philo.], philosophers);
		return (1);
	}
	return (0);
}

//if sm1 died//if curr phil is dead then set dining->dead = 1//if they all ate
int	check_death(t_philosopher *philo)
{
	//printf("time in ms: %llu\nphilo last meal: %llu\ntt_die: %llu\n", time_in_ms, philo->last_meal, philo->dining->tt_die);
	//printf("time - last meal: %llu\n", time_in_ms() - philo->last_meal);
	if ((time_in_ms() - philo->last_meal) >= philo->dining->tt_die)
	{
		//printf("\n\nMUST END HERE\n\n");
		print(philo, (time_in_ms() - philo->dining->start_time), "has died");
		pthread_mutex_lock(&philo->dining->dead_m);
		philo->dining->dead = 1;
		pthread_mutex_unlock(&philo->dining->dead_m);
		exit(1);
		//free_param(philo, philo->mutex, philosophers);
		return (1);
	}
	return (0);
}

	//maybe implement this in done_dining if all_ate will be a flag
	// int	i;

	// i = 0;
	// printf("must eat: %d\nphilo all ate: %d\n", philo->dining->must_eat, philo->dining->all_ate);
	// if (philo->dining->must_eat != -1)
	// 	return (0);
	// while (i < philo->dining->no_of_philo)
	// {
	// 	pthread_mutex_lock(&philo->dining->meals_m);
	// 	if (philo->dining->philos[i].c_ate < philo->dining->must_eat)
	// 	{
	// 		pthread_mutex_unlock(&philo->dining->meals_m);
	// 		break ;		
	// 	}
	// 	pthread_mutex_unlock(&philo->dining->meals_m);
	// 	usleep(50);
	// 	i = (i + 1) % philo->dining->no_of_philo;
	// }
	// pthread_mutex_lock(&philo->dining->meals_m);
	// if (i == philo->dining->no_of_philo)
	// {
	// 	printf("\n\nDONE eating\n\n");
	// 	philo->dining->all_ate = 1;
	// 	pthread_mutex_unlock(&philo->dining->meals_m);
	// 	return (1);
	// }
	// pthread_mutex_unlock(&philo->dining->meals_m);