/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 12:47:53 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/08 20:00:59 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void		init_page(t_page *page, t_page_type type, size_t size)
{
	page->next = 0;
	page->prev = 0;
	page->type = type;
	page->size = size;
	page->free = 0;
}

void		init_page_blocks(t_page *page)
{
	t_block	*b;

	page->free = (t_block *)(page + 1);
	b = page->free;
	b->size = page->size - sizeof(t_block) * 2 - sizeof(t_page);
	b->is_free = 1;
	b->prev = 0;
	b->next = (t_block *)((t_byte *)(b + 1) + b->size);
	b->next->size = 0;
	b->next->is_free = 0;
	b->next->prev = b;
	b->next->next = 0;
}

t_page		*init_new_page(t_page_type type, size_t size)
{
	t_page	*p;

	p = map_page(size);
	init_page(p, type, size);
	init_page_blocks(p);
	return (p);
}
