/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 16:00:37 by oyagci            #+#    #+#             */
/*   Updated: 2018/04/25 16:00:38 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	swap(char *a, char *b)
{
	char c;

	c = *a;
	*a = *b;
	*b = c;
}

void		ft_strrev(char *s)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	i = 0;
	while (i < len / 2)
	{
		swap(&s[i], &s[len - i - 1]);
		i += 1;
	}
}
