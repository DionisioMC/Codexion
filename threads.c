/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:07:29 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/21 16:39:13 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_thread(void *work)
{
	t_work	*work_original;

	work_original = (t_work *) work;
	work_original->coder->last_compile_start = get_time_ms();
	printf("%ld\n", work_original->coder->last_compile_start);
	return (NULL);
}

void	gen_coder_threads(t_coder *coders, t_settings *config)
{
	int				i;
	t_simulation	sim;
	t_work  work[config->number_of_coders];
	pthread_t		thread;

	i = 0;
	sim.coders = coders;
	sim.config = config;
	while (i < config->number_of_coders)
	{
		work[i].sim = &sim;
		work[i].coder = &coders[i];
		pthread_create(&thread, NULL, coder_thread, &work);
		i++;
	}
}