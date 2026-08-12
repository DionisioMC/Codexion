/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 15:01:55 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/12 16:35:57 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	arg_error(void)
{
	printf("Incorrect usage\n");
	printf("Usage: ./codexion number_of_coders time_to_burnout "
		"time_to_compile time_to_debug time_to_refactor "
		"number_of_compiles_required dongle_cooldown scheduler\n");
	exit(1);
}

t_settings	*parser(int argc, char **argv)
{
	t_settings	*config;

	config = malloc(sizeof(t_settings));
	if (argc -1 != 8)
		arg_error();
	if (argv[1] > 0)
		config->number_of_coders = atoi(argv[1]);
	else if (argv[2] > 0)
		config->time_to_burnout = atoi(argv[2]);
	else if (argv[3] > 0)
		config->time_to_compile = atoi(argv[3]);
	else if (argv[4] > 0)
		config->time_to_debug = atoi(argv[4]);
	else if (argv[5] > 0)
		config->time_to_refactor = atoi(argv[5]);
	else if (argv[6] > 0)
		config->number_of_compiles_required = atoi(argv[6]);
	else if (argv[7] > 0)
		config->dongle_cooldown = atoi(argv[7]);
	else if (strcmp(argv[8], "fifo") && strcmp(argv[8], "edf"))
		config->scheduler = argv[8];
	else
		exit(1);
	return (config);
}
