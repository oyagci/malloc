/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 07:02:14 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/10 10:06:26 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft/includes/libft.h"

extern t_page_info	g_pools[3];

int					resize_block(t_block *b, size_t s)
{
	t_block	*neighboor;

	neighboor = (t_block *)((t_byte *)(b + 1) + b->size);

	if (neighboor->size >= s && neighboor->is_free && neighboor->prev)
	{
		neighboor->prev->next = neighboor->next;
		b->size += neighboor->size + sizeof(t_block);
		return (1);
	}
	return (0);
}

void				*realloc_internal(void *ptr, size_t size, t_page_info *pools)
{
	t_block	*old;
	void	*newp;

	if (!ptr)
		return (malloc_internal(size, pools));
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

void				*reallocf(void *ptr, size_t size)
{
	return (realloc(ptr, size));
}
