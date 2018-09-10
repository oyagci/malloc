/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 07:02:14 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/10 16:28:03 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft/includes/libft.h"

extern t_page_info	g_pools[3];

int					resize_block(t_block *b, size_t s)
{
	t_block	*neighboor;

	neighboor = (t_block *)((t_byte *)(b + 1) + b->size);
	if (b->size + neighboor->size + sizeof(t_block) >= s
		&& neighboor->is_free
		&& neighboor->prev)
	{
		if (neighboor->prev)
			neighboor->prev->next = neighboor->next;
		if (neighboor->next)
			neighboor->next->prev = neighboor->prev;
		b->size += neighboor->size + sizeof(t_block);
		return (1);
	}
	return (0);
}

int					is_block_in_page(t_block *block, t_page *page)
{
	t_block	*b;

	b = (t_block *)(page + 1);
	while (b->size != 0)
	{
		if (b == block)
			return (1);
		b = (t_block *)((t_byte *)(b + 1) + b->size);
	}
	return (0);
}

int					check_ptr(void *ptr, t_page_info *pools)
{
	t_page	*p;
	t_block	*b;
	int		i;

	if ((void*)ptr < (void*)sizeof(t_block))
		return (0);
	b = (t_block *)ptr - 1;
	i = 0;
	while (i < 3)
	{
		p = pools[i].start;
		while (p)
		{
			if (is_block_in_page(b, p))
				return (1);
			p = p->next;
		}
		i += 1;
	}
	return (0);
}

void				*realloc_internal(
		void *ptr,
		size_t size,
		t_page_info *pools
)
{
	t_block	*old;
	void	*newp;

	if (!ptr)
		return (malloc_internal(size, pools));
	if (!check_ptr(ptr, pools))
		return (0);
	old = (t_block *)ptr - 1;
	if (old->size >= size)
		return (ptr);
	if (resize_block(old, size))
		return (ptr);
	newp = malloc_internal(size, pools);
	if (newp == 0)
		return (0);
	ft_bzero(newp, size);
	ft_memcpy(newp, ptr, old->size);
	return (newp);
}

void				*realloc(void *ptr, size_t size)
{
	return (realloc_internal(ptr, size, g_pools));
}
