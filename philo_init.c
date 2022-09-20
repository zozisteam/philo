/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 15:01:04 by alalmazr          #+#    #+#             */
/*   Updated: 2022/09/20 12:34:04 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_dining(char **argv, t_dining *dining)
{
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0
		|| ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0)
		return (1);
	if (argv[5] && ft_atoi(argv[5]) <= 0)
		return (1);
	dining->no_of_philo = ft_atoi(argv[1]);
	dining->tt_die = ft_atoi(argv[2]);
	dining->tt_eat = ft_atoi(argv[3]);
	dining->tt_sleep = ft_atoi(argv[4]);
	if (argv[5])
		dining->must_eat = ft_atoi(argv[5]);
	else
		dining->must_eat = -1;
	dining->dead = 0;
	dining->all_ate = 0;
	return (0);
}

int	init_mutex(t_dining *dining)
{
	int	i;

	i = 0;
	while (i < dining->no_of_philo)
	{
		if (pthread_mutex_init(&dining->forks_mutex[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&dining->meals_m, NULL))
		return (1);
	if (pthread_mutex_init(&dining->dead_m, NULL))
		return (1);
	if (pthread_mutex_init(&dining->print_mutex, NULL))
		return (1);
	return (0);
}

int	init_philosophers(t_dining *dining)
{
	int	i;

	i = 0;
	while (i < dining->no_of_philo)
	{
		dining->philos[i].id = i + 1;
		dining->philos[i].c_ate = 0;
		if (i == dining->no_of_philo - 1)
			dining->philos[i].r_fork_id = 0;
		else
			dining->philos[i].r_fork_id = i + 1;
		dining->philos[i].l_fork_id = i;
		dining->philos[i].dining = dining;
		dining->philos[i].last_meal = 0;
		i++;
	}
	return (0);
}

int	init(char **argv, t_dining *dining)
{
	if (init_dining(argv, dining))
		return (1);
	if (init_mutex(dining))
		return (1);
	if (init_philosophers(dining))
		return (1);
	dining->start = time_in_ms();
	return (0);
}
