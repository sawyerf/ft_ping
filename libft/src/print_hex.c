#include "libft.h"

void print_hex(unsigned char *addr, size_t size)
{
	int i;

	i = 0;
	ft_printf("\n %d bytes\n00 - ", size);
	while (size)
	{
		ft_printf("%.2X ", *addr);
		addr++;
		size--;
		i++;
		if (!(i % 10))
			ft_printf("\n%d - ", i);
	}
	ft_printf("\n");
}
