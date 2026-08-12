/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:36:56 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/12 16:54:57 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_list	*ft_lstnew(t_coder *content)
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

int	main(int argc, char **argv)
{
	int			i;
	t_settings	*config;
	t_list		**coders;
	t_coder		*coder;

	i = 0;
	config = parser(argc, argv);
	coders = (t_list **)malloc(sizeof(t_list *));
	while (i < config->number_of_coders)
	{
		coder = malloc(sizeof(t_coder));
		coder->number = i + 1;
		ft_lstadd_front(coders, ft_lstnew(coder));
		i++;
	}
}
