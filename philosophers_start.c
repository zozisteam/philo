/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_start.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:29:37 by alalmazr          #+#    #+#             */
/*   Updated: 2022/08/15 19:56:13 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_dining(int argc, char **argv, t_dining *dining)
{
	//checking that all args when converted to int are valid
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0 ||  ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0)
		return (1);
	if (argv[5] && ft_atoi(argv[5]) <= 0)
		return (1);
	//check that the input is ok so that we're sure code below has no errors
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


int	init_philosophers(t_dining *dining)
{
	int	i;
	int rfork;

	i = 0;
	printf("-----INIT_PHILOSOPHERS()----\n");
	while (i < dining->no_of_philo) //add flag for even odd so they share forks//draw this to double check
	{
		dining->philos[i].id = i;
		printf("philo id: %d\n", dining->philos[i].id);
		dining->philos[i].c_ate = 0;
		if (i == dining->no_of_philo - 1)
			dining->philos[i].r_fork_id = 0;
		else
		{
			dining->philos[i].r_fork_id = i + 1;
		}
		dining->philos[i].l_fork_id = i;
		printf("l_fork: %d\tr_fork: %d\n", dining->philos[i].l_fork_id, dining->philos[i].r_fork_id);
		dining->philos[i].dining = dining;
		dining->philos[i].last_meal = 0;
		i++;
	}
	return 0;
}

int	init_mutex(t_dining *dining)
{
	int	i;

	i = 0;
	while (i < dining->no_of_philo)
	{
		if (pthread_mutex_init(&dining->forks_mutex[i], NULL))
			return (1);
		//dining->forks[i] = -1;
		printf("init fork mutex [%d]\n",i+1);
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

int	init(int argc, char **argv, t_dining *dining)
{
	dining->start_time = time_in_ms();
	if (init_dining(argc, argv, dining))
		return (1);
	if (init_mutex(dining))
		return (1);
	if (init_philosophers(dining))
		return (1);
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
		printf("pthread_create philo [%d]\n",i+1);
		usleep(20);
		i++;
	}
	 /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */
	//protecting threads  //waiting for children processes to finish before parent process ends
	i = 0;
	while (i < dining->no_of_philo)
	{
		if (pthread_join(philosophers[i].thread_id, NULL))
			return (1);
		printf("pthread_join philo [%d]\n",i+1);
		i++;
	}
	return (0);
}