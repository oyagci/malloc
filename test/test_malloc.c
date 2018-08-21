#include <criterion/criterion.h>
#include "malloc.h"

Test(malloc, basic_char)
{
	char	*p = malloc_internal(sizeof(char));

	cr_assert(p != 0);
	*p = 'c';
}
