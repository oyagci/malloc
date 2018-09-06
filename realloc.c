/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 13:23:18 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/06 13:42:58 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft/includes/libft.h"

void	*realloc(void *ptr, size_t size)
{
	if (!ptr)
		return (malloc(size));

	t_block	*old = (t_block *)ptr - 1;

	if (old->size >= size)
		return (ptr);

	void	*newp = malloc(size);

	if (newp == 0)
		return (0);
	ft_bzero(newp, size);
	ft_memcpy(newp, ptr, old->size);
	return (newp);
}

void	*reallocf(void *ptr, size_t size)
{
	return (realloc(ptr, size));
}
