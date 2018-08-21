#ifndef MALLOC_H
# define MALLOC_H

#include <string.h>

void	*malloc(size_t size);
void	*calloc(size_t size, size_t n);
void	*realloc(void *ptr, size_t size);

#endif
