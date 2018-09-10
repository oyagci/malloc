/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 07:02:08 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/10 10:06:57 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft/includes/libft.h"
#include <stdlib.h>

void	*calloc(size_t size, size_t n)
{
	void	*mem;

	mem = malloc(size * n);
	if (size == 0)
		ft_bzero(mem, 16);
	else
		ft_bzero(mem, size * n);
	return (mem);
}
