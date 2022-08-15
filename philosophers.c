/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 19:50:19 by alalmazr          #+#    #+#             */
/*   Updated: 2022/08/11 14:24:06 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"



long long	time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000LL + time.tv_usec / 1000);
}

int main(int argc, char **argv)
{
	t_dining *dining;

	if (argc < 5 || argc > 6)
		exit(1);
	dining = (t_dining *)malloc(sizeof(t_dining));
	if (init(argc, argv, dining))
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