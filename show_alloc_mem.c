/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/06 12:02:59 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/13 12:09:47 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

extern t_page_info	g_pools[3];

void				show_page_title(t_page_type type, void *addr)
{
	if (type == TINY)
		ft_putstr("TINY : ");
	else if (type == SMALL)
		ft_putstr("SMALL : ");
	else if (type == LARGE)
		ft_putstr("LARGE : ");
	print_addr(addr);
	ft_putstr(" - ");
	print_addr(addr + ((t_page*)addr)->size);
	ft_putchar('\n');
}

void				show_alloc_page(t_page *p)
{
	t_block	*b;

	b = (t_block *)(p + 1);
	if (b->size == 0)
		return ;
	show_page_title(p->type, p);
	while (b->size != 0)
	{
		if (!b->is_free)
		{
			print_addr(b);
			ft_putstr(" - ");
			print_addr((t_byte *)(b) + b->size);
			ft_putstr(" : ");
			ft_putnbr(b->size);
			ft_putstr(" octets");
			ft_putchar('\n');
		}
		b = (t_block *)((t_byte *)(b + 1) + b->size);
	}
}

void				show_alloc_pages(t_page *p)
{
	while (p)
	{
		show_alloc_page(p);
		p = p->next;
	}
}

void				show_alloc_mem_internal(t_page_info pools[3])
{
	int	i;

	i = 0;
	while (i < 3)
	{
		show_alloc_pages(pools[i].start);
		i += 1;
	}
}

void				show_alloc_mem()
{
	int	i;

	i = 0;
	while (i < 3)
	{
		show_alloc_pages(g_pools[i].start);
		i += 1;
	}
}
