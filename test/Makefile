all:
	gcc free.c init.c test_malloc.c test_malloc_internal.c find_free_block.c \
	    malloc_large.c malloc_init.c merge_block.c expand_block.c test_realloc.c ../*.c \
	    -I ~/.brew/include -lcriterion -L ~/.brew/lib -lft -L ../libft/ -I ../libft/includes -g
	./a.out -j1
