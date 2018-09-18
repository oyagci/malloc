/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_free_blocks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 12:38:51 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/18 09:45:05 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	try_merge(t_block *b)
{
	t_block	*next;

	next = (t_block *)((t_byte *)(b + 1) + b->size);
	if (b->is_free && next->is_free)
	{
		b->size = b->size + next->size + sizeof(t_block);
		b->next = next->next;
		b->next->prev = b;
		next->prev = (void*)0xDEADBEEF;
		next->next = (void*)0xDEADBEEF;
		try_merge(b);
	}
}

void	merge_pools_free_blocks(t_page_info *pool)
{
	t_block	*b;
	t_page	*p;

	p = pool->start;
	while (p)
	{
		b = (t_block *)(p + 1);
		while (b->size != 0)
		{
			if (b->is_free)
				try_merge(b);
			b = (t_block *)((t_byte *)(b + 1) + b->size);
		}
		p = p->next;
	}
}

void	merge_free_blocks(t_page_info *pools)
{
	static int	count = 10;
	int			i;

	count -= 1;
	if (count > 0)
		return ;
	else
		count = 10;
	i = 0;
	while (i < 3)
	{
		merge_pools_free_blocks(pools + i);
		i += 1;
	}
}
