/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:59:17 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/17 17:00:26 by dcoelho          ###   ########.fr       */
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

void	error_and_exit(t_settings *config)
{
	free(config);
	exit(1);
}
