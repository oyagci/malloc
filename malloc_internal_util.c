/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal_util.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 12:46:18 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/11 15:11:26 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void		remove_block(t_block **free_list, t_block *to_remove)
{
	if (to_remove == *free_list)
	{
		*free_list = to_remove->next;
		to_remove->next->prev = 0;
	}
	else
	{
		if (to_remove->prev)
			to_remove->prev->next = to_remove->next;
		if (to_remove->next)
			to_remove->next->prev = to_remove->prev;
	}
}

void		add_remainder_to_free_list(
		t_block **free_list,
		t_block *b,
		t_block *rmder
)
{
	if (b == *free_list)
		*free_list = rmder;
	else
	{
		rmder->prev = b->prev;
		rmder->prev->next = rmder;
	}
	rmder->next = b->next;
	if (rmder->next)
		rmder->next->prev = rmder;
}
