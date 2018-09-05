/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 13:21:07 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/05 12:49:40 by oyagci           ###   ########.fr       */
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

typedef char				t_byte;
typedef int					t_page_type;
typedef struct s_page		t_page;
typedef struct s_block		t_block;
typedef struct s_page_info	t_page_info;

struct						s_page
{
	t_page_type			type;
	size_t				size;
	t_page				*next;
	t_page				*prev;
	t_block				*free;
	char				dummy;
};

struct						s_block
{
	size_t				size;
	int					is_free;
	t_block				*prev;
	t_block				*next;
};

struct						s_page_info
{
	t_page_type	type;
	t_page		*start;
};

void						*malloc(size_t size);
void						*calloc(size_t size, size_t n);
void						*realloc(void *ptr, size_t size);

void						*malloc_internal(size_t size, t_page_info *pools);
void						*malloc_large(t_page_info *pool, size_t size);
void						*map_page(size_t size);
t_page						*init_new_page(t_page_type type, size_t size);
void						init_page(t_page *page, t_page_type type,
		size_t size);
void						init_page_blocks(t_page *page);
t_block						*find_free_block(t_page_info *pinfo, size_t size);

void						free_internal(void *ptr, t_page_info *pools);
void						add_block_to_free_list(t_block *fblock,
		t_page_info *pinfo);

/*
** malloc_internal utils
*/

void						remove_block(t_block **free_list,
		t_block *to_remove);
void						add_remainder_to_free_list(
		t_block **free_list,
		t_block *b,
		t_block *rmder
);
void						append_page_to_pool(t_page_info *pool);

int							round_up(int num, int multiple);

#endif
