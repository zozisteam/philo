/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 19:30:39 by alalmazr          #+#    #+#             */
/*   Updated: 2022/08/06 00:45:01 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* routine() {
    printf("Hello from threads\n");
  //  usleep(1);
    printf("Ending thread\n");
}

int main() {
    pthread_t p1;
	int i;

	i = 0;
	while(i < 30)
	{
    	if (pthread_create(&p1, NULL, &routine, NULL) != 0) 
        	return 1;
		//  if (pthread_join(p1, NULL) != 0) 
        //return 3;
		i++;
    }

  
    

    return 0;
}