/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 14:29:12 by alalmazr          #+#    #+#             */
/*   Updated: 2022/08/15 20:22:31 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

struct	s_philosophy;

typedef struct s_philosopher
{
	int						id;
	int						c_ate;
	int						l_fork_id;
	int						r_fork_id;
	long long				last_meal;
	struct s_dining			*dining;
	pthread_t				thread_id;
}							t_philosopher;

typedef struct s_dining
{
	int					no_of_philo;
	int					must_eat;
	//if they dont have to eat a specific amount of times then: must_eat:  -1. 

	//time to die, eat, sleep
	long long			tt_die;
	long long			tt_eat;
	long long			tt_sleep;

	//flags dead & all_ate
	int					dead;
	int					all_ate;
	
	long long			start_time;
	pthread_mutex_t		dead_m;
	pthread_mutex_t		meals_m;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		forks_mutex[250];
	//mutex for each philosopher

	int					forks[250];
	//value for each fork (based on whos using and philos->id)

	t_philosopher		philos[250];
	//all the philosophers
}						t_dining;

//dine
void		*dine(void *phil);
int			done_dining(t_philosopher *philo);
int			check_death(t_philosopher *philo);
int			enter_philo(t_dining *dining, t_philosopher *philo);
int			philo_sleep(t_philosopher *philo, int sleep);

int			init_mutex(t_dining *dining);
int			start(t_dining *dining);
int			init_dining(int argc, char **argv, t_dining *dining);
int			init(int argc, char **argv, t_dining *dining);
int			init_philosophers(t_dining *dining);


long long	time_in_ms(void);
int			ft_atoi(const char *str);
void		print(t_philosopher *philo, unsigned long time, char *is_doing);
int			destroy_mutex(t_dining	*dining);

#endif
