#include "malloc.h"
#include "libft/includes/libft.h"

// void	*realloc(void *ptr, size_t size)
// {
// 	return (0);
// }

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
