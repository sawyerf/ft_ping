#include "libft.h"

double ft_sqrt(double nb)
{
	double x;
	double y;
	double prcs;

	x = nb;
	y = 1;
	prcs = 0.00000001;
	while (x - y > prcs) {
		x = (x + y) / 2;
		y = nb / x;
	}
	return x;
}
