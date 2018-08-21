#include "malloc.h"
#include "libft/includes/libft.h"

void	*calloc(size_t size, size_t n)
{
	void	*mem;

	mem = malloc(size * n);
	ft_bzero(mem, size * n);
	return (mem);
}
