/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 07:02:08 by oyagci            #+#    #+#             */
/*   Updated: 2018/08/24 07:02:09 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft/includes/libft.h"

void	*calloc(size_t size, size_t n)
{
	void	*mem;

	if ((mem = malloc(size * n)) == NULL)
		return (NULL);
	ft_bzero(mem, size * n);
	return (mem);
}
