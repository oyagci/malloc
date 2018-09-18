/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal_util.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 12:46:18 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/18 13:51:08 by oyagci           ###   ########.fr       */
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
		t_block *big,
		t_block *rmder
)
{
	if (big == *free_list)
	{
		rmder->prev = 0;
		*free_list = rmder;
	}
	else
	{
		rmder->prev = big->prev;
		rmder->prev->next = rmder;
	}
	rmder->next = big->next;
	if (rmder->next)
		rmder->next->prev = rmder;
}
