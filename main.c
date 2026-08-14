/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:36:56 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/14 14:56:29 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}

void	ft_lstclear(t_list **lst)
{
	t_list	*current;
	t_list	*next;

	if (lst)
	{
		current = *lst;
		while (current)
		{
			next = current->next;
			free(current->content);
			free(current);
			current = next;
		}
		*lst = NULL;
	}
}

void	gen_coders_and_dongles(t_list **coders, t_list **dongles,
	t_settings *config)
{
	int			i;
	t_coder		*coder;
	t_dongle	*dongle;

	i = 0;
	*coders = NULL;
	*dongles = NULL;
	while (i < config->number_of_coders)
	{
		coder = malloc(sizeof(t_coder));
		coder->number = i + 1;
		ft_lstadd_front(coders, ft_lstnew(coder));
		dongle = malloc(sizeof(t_dongle));
		dongle->id = i + 1;
		ft_lstadd_front(dongles, ft_lstnew(dongle));
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_settings	*config;
	t_list		**coders;
	t_list		**dongles;

	config = parser(argc, argv);
	coders = (t_list **)malloc(sizeof(t_list *));
	dongles = (t_list **)malloc(sizeof(t_list *));
	if (coders && dongles)
	{
		gen_coders_and_dongles(coders, dongles, config);
		free(config);
		ft_lstclear(coders);
		ft_lstclear(dongles);
		free(coders);
		free(dongles);
	}
	else
	{
		free(config);
		exit(1);
	}
}
