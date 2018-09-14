/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_free_blocks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 12:38:51 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/13 15:36:29 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <assert.h>

void	try_merge(t_block *b)
{
	int count = 0;
	t_block	*next;

	ft_putendl_fd("--- try_merge", 2);
	ft_putnbr_fd(b->size + sizeof(t_block), 2);
	ft_putchar_fd('\n',2);

	while (42)
	{
		count++;
		next = (t_block *)((t_byte *)(b + 1) + b->size);
		if (next->is_free)
		{
			ft_putstr_fd("merging ", 2);
			ft_putnbr_fd(next->size + sizeof(t_block), 2);
			ft_putchar_fd('\n', 2);

			assert(next->prev == b);
			assert(b->next == next);
			b->size = b->size + next->size + sizeof(t_block);
			b->next = next->next;
			if (b->next)
				b->next->prev = b;
			next->prev = (void*)0xDEADBEEF;
			next->next = (void*)0xDEADBEEF;
		}
		else
			break ;
	}

	ft_putnbr_fd(b->size + sizeof(t_block), 2);
	ft_putchar_fd('\n',2);
	ft_putchar_fd('\n',2);
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
	ft_putendl_fd("merge_free_blocks", 2);
	i = 0;
	while (i < 3)
	{
		merge_pools_free_blocks(pools + i);
		i += 1;
	}
}
