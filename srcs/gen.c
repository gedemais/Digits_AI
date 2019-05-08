#include "../includes/main.h"

double 	*ft_cpy_weights(double *dest, double *src)
{
	int 	i;

	i = 0;
	while (i < 13002)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}

double 	*ft_set_rand(double *weights, double min, double max)
{
	int 	i = 0;

	while (i < 13002)
	{
		weights[i] = random_dbl(min, max);
		i++;
	}
	return (weights);
}

int 	ft_is_just(double *output, int label)
{
	int 	i = 0;

	if (output[label] < 0.5)
		return (0);
	while (i < 10)
	{
		if (i != label)
			if (output[i] >= 0.5)
				return (0);
		i++;
	}
	return (1);
}
