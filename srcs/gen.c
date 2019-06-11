#include "../includes/main.h"

float 	*ft_cpy_weights(float *dest, float *src)
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

float 	*ft_set_rand(float *weights, float min, float max)
{
	int 	i = 0;

	while (i < 13002)
	{
		weights[i] = random_dbl(min, max);
		i++;
	}
	return (weights);
}

int 	ft_is_just(float *output, int label)
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
