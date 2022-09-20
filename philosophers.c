/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 19:50:19 by alalmazr          #+#    #+#             */
/*   Updated: 2022/09/20 16:45:49 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	eat(t_dining *dining, t_philosopher *philo)
{
	if (check_death_solo(philo))
		return (1);
	if (take_forks(dining, philo))
		return (1);
	if (check_death_solo(philo) || check_ate(philo))
		return (1);
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
	while (1)
	{
		if (dining->no_of_philo % 2 == 1 && philo->id % 2 == 1)
		{
			usleep(100);
		}
		else if (dining->no_of_philo % 2 == 0 && philo->id % 2 == 0)
		{
			usleep(100);
		}
		if (eat(dining, philo))
			return (0);
		philo_idle(philo, dining->tt_sleep);
		print(philo, (time_in_ms() - philo->dining->start), "is sleeping");
		print(philo, (time_in_ms() - philo->dining->start), "is thinking");
		if (check_death_solo(philo) || check_ate(philo))
			return (0);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_dining	*dining;

	if (argc < 5 || argc > 6)
		exit(1);
	dining = (t_dining *)malloc(sizeof(t_dining));
	if (init(argv, dining))
		return (-1);
	if (!start(dining))
	{
		destroy_mutex(dining);
		free(dining);
		return (0);
	}
	destroy_mutex(dining);
	free(dining);
	return (0);
}
