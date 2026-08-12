/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:38:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/12 16:39:34 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>

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

typedef struct s_dongle
{
	int				id;
	int				cooldown
}	t_dongle;

typedef struct s_coder
{
	int			number;
	int			last_compile_start;
	int			compile_count;
	pthread_t	thread;
	t_dongle	*l_dongle;
	t_dongle	*r_dongle;
}	t_coder;

typedef struct s_list
{
	t_coder			*content;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;

t_list		*ft_lstnew(t_coder *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_atoi(const char *nptr);
t_settings	*parser(int argc, char **argv);

#endif