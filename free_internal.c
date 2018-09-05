/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_internal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 18:54:39 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/05 13:39:15 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

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

t_page	*find_parent_page(t_block *b, t_page *first)
{
	while (first)
	{
		if (b > (t_block *)first &&
			b < (t_block *)((t_byte *)first + first->size))
			return (first);
		first = first->next;
	}
	return (first);
}

void	add_block_to_free_list(t_block *fblock, t_page_info *pinfo)
{
	t_page	*p;
	t_block	*b;

	if (0 == (p = find_parent_page(fblock, pinfo->start)))
		return ;
	b = p->free;
	while (b->size != 0 && b < fblock)
		b = b->next;
	if (p->free == b)
	{
		fblock->next = b;
		fblock->next->prev = fblock;
		p->free = fblock;
	}
	else
	{
		fblock->prev = b->prev;
		fblock->prev->next = fblock;
		fblock->next = b;
		b->prev = fblock;
	}
}

#include <pthread.h>
extern pthread_mutex_t lock;

void	free_internal(void *ptr, t_page_info *pools)
{
	t_block	*ptr_b;

	if (!ptr)
		return ;
	pthread_mutex_lock(&lock);
	ptr_b = (t_block *)ptr - 1;
	if (!check_block_ptr(ptr_b, pools))
	{
		pthread_mutex_unlock(&lock);
		return ;
	}
	ptr_b->is_free = 1;
	add_block_to_free_list(ptr_b, pools);
	pthread_mutex_unlock(&lock);
}
