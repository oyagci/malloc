/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <student@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 12:55:38 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/06 13:24:37 by oyagci           ###   ########.fr       */
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
