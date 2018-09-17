/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_internal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 18:54:39 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/17 15:22:16 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <pthread.h>
#include <sys/mman.h>

extern pthread_mutex_t	g_lock;
extern t_page_info		g_pools[3];

int		check_block_ptr(t_block *b, t_page_info *pinfo)
{
	t_block	*n;
	t_page	*p;
	int		i;

	i = 0;
	while (i < 3)
	{
		p = pinfo[i].start;
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
		i += 1;
	}
	return (0);
}

t_page	*is_in_pool(t_block *b, t_page *first)
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

t_page	*find_parent_page(t_block *b, t_page_info *pools)
{
	t_page	*page;
	int i;

	i = 0;
	while (i < 3)
	{
		if (NULL != (page = is_in_pool(b, pools[i].start)))
			return (page);
		i += 1;
	}
	return (0);
}

void	add_block_to_free_list(t_block *fblock, t_page_info *pinfo)
{
	t_page	*p;
	t_block	*b;

	if (0 == (p = find_parent_page(fblock, pinfo)))
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

void	unmap_free_pages(t_page_info *pools)
{
	t_page	*p;
	t_block	*b;
	int		i;
	int		is_empty;
	t_page	*next;

	i = 0;
	is_empty = 1;
	while (i < 3)
	{
		if (pools[i].start)
		{
			p = pools[i].start->next;
			while (p)
			{
				next = p->next;
				is_empty = 1;
				b = (t_block *)(p + 1);
				while (b->size != 0)
				{
					if (!b->is_free)
					{
						is_empty = 0;
						break ;
					}
					b = (t_block *)((t_byte *)(b + 1) + b->size);
				}
				if (is_empty)
				{
					if (p->prev)
					{
						p->prev->next = p->next;
						if (p->next)
							p->next->prev = p->prev;
					}
					else
					{
						pools[i].start = p->next;
						if (p->next)
							p->next->prev = 0;
					}
					munmap(p, p->size);
				}
				p = next;
			}
		}
		i += 1;
	}
}

void	free_internal(void *ptr, t_page_info *pools)
{
	t_block	*ptr_b;

	if (!ptr)
		return ;
	pthread_mutex_lock(&g_lock);
	ptr_b = (t_block *)ptr - 1;
	if (!check_block_ptr(ptr_b, pools))
	{
		pthread_mutex_unlock(&g_lock);
		return ;
	}
	ptr_b->is_free = 1;
	add_block_to_free_list(ptr_b, pools);
	unmap_free_pages(pools);
	merge_free_blocks(pools);
	pthread_mutex_unlock(&g_lock);
}
