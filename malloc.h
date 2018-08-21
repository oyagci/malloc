/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 11:12:19 by oyagci            #+#    #+#             */
/*   Updated: 2018/08/21 11:16:56 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <string.h>

# define M			2097152
# define N			16777216
# define TINY		992
# define SMALL		126992
# define LARGE		126993

# define TINY_RES	16
# define SMALL_RES	512

typedef int				t_page_type;
typedef struct s_page	t_page;
typedef struct s_block	t_block;

struct					s_page
{
	t_page_type			type; /* TINY or SMALL or LARGE */
	t_page				*next;
	t_page				*prev;
	t_block				*free; /* First free block in the page */
};

struct					s_block
{
	size_t				size;
	int					is_free;
	t_block				*prev;
	t_block				*next;
};

void					*malloc(size_t size);
void					*calloc(size_t size, size_t n);
void					*realloc(void *ptr, size_t size);

#endif
