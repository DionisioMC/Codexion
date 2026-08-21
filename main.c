/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:36:56 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/21 12:08:42 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_coders(t_coder *coders, t_settings *config)
{
	int	i;

	i = 0;
	while (i < config->number_of_coders)
	{
		if (coders[i].r_dongle)
		{
			free(coders[i].r_dongle);
			coders[i].r_dongle = NULL;
		}
		i++;
	}
	free(coders);
}

t_dongle	*gen_dongle(int i, t_settings *config, t_coder *coders)
{
	t_dongle	*dongle;

	dongle = (t_dongle *) malloc(sizeof(t_dongle));
	if (dongle)
	{
		dongle->id = i + 1;
		dongle->cooldown = config->dongle_cooldown;
		return (dongle);
	}
	else
	{
		free_coders(coders, config);
		free(config);
		exit(1);
	}
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	gen_coders_and_dongles(t_coder *coders, t_settings *config)
{
	int				i;
	t_coder			coder;

	i = 0;
	while (i < config->number_of_coders)
	{
		coder.number = i + 1;
		coder.last_compile_start = get_time_ms();
		coder.compile_count = 0;
		coder.l_dongle = NULL;
		coder.r_dongle = gen_dongle(i, config, coders);
		if (i > 0 && i != (config->number_of_coders - 1))
			coder.l_dongle = coders[i - 1].r_dongle;
		else if (i > 0)
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
		gen_coder_threads(coders, config);
		print_coders(coders, config->number_of_coders);
		free_coders(coders, config);
		free(config);
	}
	else
	{
		free(config);
		exit(1);
	}
}
