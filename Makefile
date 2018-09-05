# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oyagci <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/12 13:09:17 by oyagci            #+#    #+#              #
#    Updated: 2018/09/05 15:58:25 by oyagci           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		= libft_malloc_$(HOSTTYPE).so

CFLAGS		= -Wall -Wextra -Werror -g

SOURCES		= malloc.c 				\
		  free.c					\
		  realloc.c					\
		  show_alloc_mem.c			\
		  malloc_internal.c			\
		  free_internal.c 			\
		  malloc_internal_util.c	\
		  malloc_large.c			\
		  init.c				\
		  calloc.c

DEPS		= malloc.h

%.o: %.c $(DEPS)
	$(CC) -fPIC -c -o $@ $< $(CFLAGS) -I libft/includes -I .

all: $(NAME)

$(NAME): $(SOURCES:.c=.o)
	make -C libft/
	$(CC) -fPIC -shared -o $(NAME) $(SOURCES:.c=.o) -L libft -lft
	ln -F -f -s $(NAME) libft_malloc.so

_test:
	$(CC) tests/main.c -o tests/tests -lcriterion -lft -L libft/ -I libft/includes -I . -I ~/.brew/include -L ~/.brew/lib ft_malloc.c

test: all _test
	./tests/tests

clean:
	rm -f *.o

fclean: clean
	rm -f *.so

re: fclean all
