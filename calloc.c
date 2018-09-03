/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 12:55:38 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/03 12:55:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft/includes/libft.h"

void	*calloc(size_t size, size_t n)
{
	void	*mem;

	mem = malloc(size * n);
	ft_bzero(mem, size * n);
	return (mem);
}
