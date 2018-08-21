/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:47:24 by oyagci            #+#    #+#             */
/*   Updated: 2018/08/21 14:58:29 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "malloc.h"

t_page_info	g_pools[3];

void	*map_page(size_t size)
{
	void	*p;

	p = mmap(
		0,
		size,
		PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE,
		0, 0
	);
	return (p);
}

t_page		*init_new_page()
{
	t_page	*p = map_page(M);

	init_page(p);
}

t_block		*find_free_block(size_t size)
{
	t_page_info		*info;

	info = &g_pools[0];
	if (!info->start)
	{
		info->start = init_new_page();
	}
}

void		*malloc_internal(size_t size)
{
	t_block	*b;

	b = find_free_block(size);
	return (b ? b + 1 : 0);
}
