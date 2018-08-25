/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_internal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 18:54:39 by oyagci            #+#    #+#             */
/*   Updated: 2018/08/25 16:20:48 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

# include <stdio.h>

extern t_page_info	g_pools[3];

int		check_block_ptr(t_block *b, t_page_info *pinfo)
{
	t_block	*n;
	t_page	*p;

	p = pinfo->start;
	while (p)
	{
		n = (t_block *)(p + 1);
		while (n->size != 0)
		{
			if (b == n && !b->is_free)
				return (1);
			n = (t_block *)((t_byte *)(n + 1) + n->size);
		}
		p = p->next;
	}
	return (0);
}

void	add_block_to_free_list(t_block *fblock, t_page_info *pinfo)
{
	t_page	*p;
	t_block	*b;

	p = pinfo->start;
	while (p)
	{
		if (fblock > (t_block *)p && fblock < (t_block *)(p + p->size))
			break ;
		p = p->next;
	}
	b = p->free;
	while (b->size != 0 && b->next < fblock)
		b = b->next;
	fblock->next = b->next;
	fblock->prev = b;
	b->next = fblock;
}

void	free_internal(void *ptr)
{
	t_block	*ptr_b;

	ptr_b = (t_block *)ptr - 1;
	if (!check_block_ptr(ptr_b, &g_pools[0]))
		return ;
	ptr_b->is_free = 1;
	add_block_to_free_list(ptr_b, &g_pools[0]);
}
