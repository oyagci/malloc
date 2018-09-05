/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_large.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 11:41:39 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/05 13:12:06 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <libft.h>
#include <unistd.h>

void	add_page_to_large(t_page_info *pool, t_page *page)
{
	t_page	*p;

	if (pool->start == 0)
		pool->start = page;
	else
	{
		p = pool->start;
		while (p->next)
			p = p->next;
		p->next = page;
		page->prev = p;
	}
}

void	*malloc_large(t_page_info *pool, size_t size)
{
	t_page	*page;
	size_t	page_size;
	t_block	*b;

	size = round_up(size, getpagesize());
	page_size = size + sizeof(t_page_info) + sizeof(t_block) * 2;
	page_size = round_up(page_size, getpagesize());
	page = init_new_page(LARGE, page_size);
	add_page_to_large(pool, page);
	b = find_free_block(pool, size);
	return (b ? b + 1 : 0);
}
