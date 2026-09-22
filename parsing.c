/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 15:01:55 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/22 17:04:38 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_int(char *str)
{
	long	value;
	int		i;

	value = 0;
	i = 0;
	while (str[i])
	{
		value = value * 10 + (str[i] - '0');
		if (value > 2147483647)
		{
			fprintf(stderr, "Parsing Error: A value exceeded "
				"the INT_MAX limits.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	are_valid_numbers(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc - 1)
	{
		j = 0;
		while (argv[i][j])
		{
			if ((argv[i][j] >= '0') && argv[i][j] <= '9')
				j++;
			else
				return (1);
		}
		if (is_int(argv[i]) == 1)
			return (1);
		if (atoi(argv[i]) == 0)
		{
			fprintf(stderr, "Parsing Error: Values need to be "
				"higher or equal to zero.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	has_valid_scheduler(char *str)
{
	if ((strcmp(str, "fifo") == 0) || (strcmp(str, "edf") == 0))
		return (0);
	fprintf(stderr, "Parsing Error: Invalid scheduler.\n");
	return (1);
}

char	**args_verify(int argc, char **argv)
{
	if (argc -1 != 8)
		return (NULL);
	if ((are_valid_numbers(argc, argv) == 1)
		|| (has_valid_scheduler(argv[8]) == 1))
		return (NULL);
	return (argv);
}

int	gen_simulation(t_simulation *sim, char **args)
{
	sim->number_of_coders = atoi(args[1]);
	sim->time_to_burnout = atoi(args[2]);
	sim->time_to_compile = atoi(args[3]);
	sim->time_to_debug = atoi(args[4]);
	sim->time_to_refactor = atoi(args[5]);
	sim->number_of_compiles_required = atoi(args[6]);
	sim->dongle_cooldown = atoi(args[7]);
	sim->scheduler = args[8];
	sim->start_time = get_time_ms();
	sim->stop = false;
	sim->request_count = 0;
	if (gen_dongles(sim) == 1)
		return (1);
	if (gen_coders(sim) == 1)
		return (1);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->log_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&sim->request_mutex, NULL))
		return (1);
	return (0);
}
