/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:36:56 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/18 17:27:59 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*gen_dongle(int i)
{
	t_dongle	*dongle;

	dongle = (t_dongle *) malloc(sizeof(t_dongle));
	dongle->id = i + 1;
	return (dongle);
}

void	gen_coders_and_dongles(t_coder *coders, t_settings *config)
{
	int				i;
	t_coder			coder;
	struct timeval	time;

	i = 0;
	while (i < config->number_of_coders)
	{
		coder.number = i + 1;
		gettimeofday(&time, NULL);
		coder.last_compile_start = time.tv_sec;
		coder.compile_count = 0;
		coder.l_dongle = NULL;
		coder.r_dongle = gen_dongle(i);
		if (i != (config->number_of_coders - 1) && config->number_of_coders > 1)
			coder.l_dongle = coders[i - 1].r_dongle;
		else if (config->number_of_coders > 1)
		{
			coder.l_dongle = coders[i - 1].r_dongle;
			coders[0].l_dongle = coder.r_dongle;
		}
		coders[i] = coder;
		i++;
	}
}

void	print_coders(t_coder *coders, int number_of_coders)
{
	int	i;

	i = 0;
	while (i < number_of_coders)
	{
		printf("Coder %d:\n", coders[i].number);
		printf("  last_compile_start: %ld\n", coders[i].last_compile_start);
		printf("  compile_count: %d\n", coders[i].compile_count);
		printf("  l_dongle: %p\n", (void *)coders[i].l_dongle);
		printf("  r_dongle: %p\n", (void *)coders[i].r_dongle);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_settings	*config;
	t_coder		*coders;

	config = parser(argc, argv);
	coders = (t_coder *)malloc(sizeof(t_coder) * config->number_of_coders);
	if (coders)
	{
		gen_coders_and_dongles(coders, config);
		print_coders(coders, config->number_of_coders);
		free(config);
		free(coders);
	}
	else
	{
		free(config);
		if (coders)
			free(coders);
		exit(1);
	}
}
