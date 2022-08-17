/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 19:50:19 by alalmazr          #+#    #+#             */
/*   Updated: 2022/08/17 16:29:39 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int main(int argc, char **argv)
{
	t_dining *dining;

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