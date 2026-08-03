/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:36:56 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/03 15:34:52 by dcoelho          ###   ########.fr       */
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

int	ft_atoi(const char *nptr)
{
	int	i;
	int	signal;

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

t_settings	*parser(int argc, char **argv)
{
	t_settings	*config;

	config = malloc(sizeof(t_settings));
	if (argc -1 != 8)
	{
		printf("Incorrect usage\n");
		printf("Usage: ./codexion number_of_coders time_to_burnout "
			"time_to_compile time_to_debug time_to_refactor "
			"number_of_compiles_required dongle_cooldown scheduler\n");
		exit(1);
	}
	config->number_of_coders = ft_atoi(argv[1]);
	config->time_to_burnout = ft_atoi(argv[2]);
	config->time_to_compile = ft_atoi(argv[3]);
	config->time_to_debug = ft_atoi(argv[4]);
	config->time_to_refactor = ft_atoi(argv[5]);
	config->number_of_compiles_required = ft_atoi(argv[6]);
	config->dongle_cooldown = ft_atoi(argv[7]);
	config->scheduler = argv[8];
	return (config);
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
	printf("%d\n", config->number_of_coders);
	while (i < config->number_of_coders)
	{
		coder->number = i + 1;
		ft_lstadd_front(coders, ft_lstnew(coder));
		i++;
	}
	printf("%d\n", (*coders)->content->number);
}
