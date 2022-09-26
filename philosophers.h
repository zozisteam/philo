/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 14:29:12 by alalmazr          #+#    #+#             */
/*   Updated: 2022/09/25 18:20:53 by alalmazr         ###   ########.fr       */
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

struct	t_dining;

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
	long long			tt_die;
	long long			tt_eat;
	long long			tt_sleep;
	int					dead;
	int					all_ate;
	long long			start;
	pthread_mutex_t		dead_m;
	pthread_mutex_t		meals_m;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		forks_mutex[250];
	int					forks[250];
	t_philosopher		philos[250];
}						t_dining;

//dine
void		*dine(void *phil);
int			check_ate(t_philosopher *philo);
int			check_death_solo(t_philosopher *philo);
int			philo_idle(t_philosopher *philo, int sleep);
void		check_finish_bg(t_philosopher *philo, t_dining *dining);
int			take_forks(t_dining *dining, t_philosopher *philo);

//init
int			init(char **argv, t_dining *dining);
int			init_mutex(t_dining *dining);
int			init_dining(char **argv, t_dining *dining);
int			init_philosophers(t_dining *dining);
int			start(t_dining *dining);

//utils
int			enter_philo(t_dining *dining, t_philosopher *philo);
long long	time_in_ms(void);
int			ft_atoi(const char *str);
void		print(t_philosopher *philo, unsigned long time, char *is_doing);
int			destroy_mutex(t_dining	*dining);

#endif
