/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:08:14 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/22 11:51:40 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_smaller_child(t_heap *heap, int i, t_simulation *sim)
{
	int		smaller_i;
	int		left_i;
	int		right_i;

	left_i = 2 * i + 1;
	right_i = 2 * i + 2;
	smaller_i = i;
	if (left_i < heap->size
		&& has_higher_priority(sim, heap->data[left_i],
			heap->data[smaller_i]))
		smaller_i = left_i;
	if (right_i < heap->size
		&& has_higher_priority(sim, heap->data[right_i],
			heap->data[smaller_i]))
		smaller_i = right_i;
	return (smaller_i);
}

void	move_up(t_heap *heap, int i, t_simulation *sim)
{
	int	parent_i;

	while (i > 0)
	{
		parent_i = (i - 1) / 2;
		if (!has_higher_priority(sim, heap->data[i], heap->data[parent_i]))
			return ;
		ft_swap(&heap->data[i], &heap->data[parent_i]);
		i = parent_i;
	}
}

void	move_down(t_heap *heap, int i, t_simulation *sim)
{
	int	smaller_i;

	while (1)
	{
		smaller_i = get_smaller_child(heap, i, sim);
		if (smaller_i == i)
			return ;
		ft_swap(&heap->data[i], &heap->data[smaller_i]);
		i = smaller_i;
	}
}

int	min_heap_push(t_simulation *sim, t_heap *heap, t_coder *new_coder)
{
	if (heap->size >= sim->number_of_coders)
	{
		fprintf(stderr, "ERROR: Heap is full.\n");
		return (1);
	}
	heap->data[heap->size] = new_coder;
	heap->size++;
	move_up(heap, heap->size - 1, sim);
	return (0);
}

t_coder	*min_heap_pop(t_heap *heap, t_simulation *sim)
{
	t_coder	*last_element;
	t_coder	*first_element;

	if (heap->size <= 0)
	{
		fprintf(stderr, "ERROR: Heap is already empty.\n");
		return (NULL);
	}
	last_element = heap->data[heap->size - 1];
	first_element = heap->data[0];
	heap->data[0] = last_element;
	heap->size--;
	move_down(heap, 0, sim);
	return (first_element);
}
