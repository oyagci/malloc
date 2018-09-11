/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_addr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 16:20:39 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/11 10:39:47 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void				print_addr_fd(void *addr, int fd)
{
	const char	*digits = "0123456789abcdef";
	size_t		nb;
	char		buffer[100];
	size_t		len;

	len = 0;
	nb = (size_t)addr;
	ft_bzero(buffer, sizeof(buffer));
	ft_putstr_fd("0x", fd);
	if (nb == 0)
		ft_putchar_fd('0', fd);
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
