/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 15:01:55 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/26 12:16:02 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	ft_atol(const char *nptr)
{
	long	i;
	long	signal;

	i = 0;
	signal = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			signal *= -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		i = i * 10 + *nptr - 48;
		nptr++;
	}
	return (i * signal);
}

int	is_valid_number(char *s)
{
	int		i;
	long	num;

	i = 0;
	num = ft_atol(s);
	while (s[i])
	{
		if (s[i] == '-' || s[i] == '+')
		{
			i++;
			continue ;
		}
		else if (s[i] < 48 || s[i] > 57)
		{
			return (0);
		}
		i++;
	}
	if (num < 0 || num > 2147483647)
	{
		return (0);
	}
	return (1);
}

void	parse_verify(t_simulation *sim, char **argv)
{
	int	*fields[7];
	int	i;

	fields[0] = &sim->number_of_coders;
	fields[1] = &sim->time_to_burnout;
	fields[2] = &sim->time_to_compile;
	fields[3] = &sim->time_to_debug;
	fields[4] = &sim->time_to_refactor;
	fields[5] = &sim->number_of_compiles_required;
	fields[6] = &sim->dongle_cooldown;
	i = 0;
	while (i < 7)
	{
		if (!is_valid_number(argv[i + 1]))
			error_and_exit(sim);
		*fields[i] = (int)ft_atol(argv[i + 1]);
		i++;
	}
	if (!strcmp(argv[8], "fifo") || !strcmp(argv[8], "edf"))
		sim->scheduler = argv[8];
	else
		error_and_exit(sim);
}

t_simulation	*parser(int argc, char **argv)
{
	t_simulation	*sim;

	if (argc -1 != 8)
		arg_error();
	sim = malloc(sizeof(t_simulation));
	if (!sim)
	{
		exit(1);
	}
	parse_verify(sim, argv);
	return (sim);
}
