/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/06 12:02:59 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/10 10:06:42 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft/includes/libft.h"

extern t_page_info	g_pools[3];

void				print_addr_fd(void *addr, int fd)
{
	char	*digits = "0123456789abcdef";
	size_t	nb = (size_t)addr;
	char	buffer[100] = { 0 };
	size_t	len = 0;

	ft_putstr_fd("0x", fd);
	while (nb > 0)
	{
		buffer[len++] = digits[(size_t)nb % 16];
		nb /= 16;
	}
	ft_strrev(buffer);
	ft_putstr_fd(buffer, fd);
}

void				print_addr(void *addr)
{
	print_addr_fd(addr, 1);
}


void				show_alloc_page(t_page *p)
{
	t_block	*b;

	b = (t_block *)(p + 1);
	while (b->size != 0)
	{
		print_addr(b);
		ft_putstr(" - ");
		print_addr((t_byte *)(b) + b->size);
		ft_putstr(" ");
		ft_putnbr(b->size);
		ft_putstr(" octets");
		ft_putchar('\n');
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

void				show_page_title(t_page_type type)
{
	if (type == TINY)
		ft_putendl("TINY");
	else if (type == SMALL)
		ft_putendl("SMALL");
	else if (type == LARGE)
		ft_putendl("LARGE");
}

void				show_alloc_mem(void)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		show_page_title(g_pools[i].type);
		show_alloc_pages(g_pools[i].start);
		i += 1;
	}
}
