/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:38:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/03 15:32:53 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>

typedef struct s_setting
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_settings;

typedef struct s_coder
{
	int	number;
}	t_coder;

typedef struct s_list
{
	t_coder			*content;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;

#endif