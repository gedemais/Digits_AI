#include "../includes/main.h"

t_neurone	**ft_init_activations(t_neurone **nw)
{
	int 	i = 0;

	while (i < NB_N_INPUT)
	{
		nw[0][i].f = &ft_sigmoid;
		nw[0][i].stop = false;
		i++;
	}
	nw[0][i].stop = false;
	nw[0][i + 1].stop = true;
	i = 0;
	while (i < NB_N_HIDDEN1)
	{
		nw[1][i].f = &ft_sigmoid;
		nw[1][i].stop = false;
		i++;
	}
	nw[1][i].stop = false;
	nw[1][i + 1].stop = true;
	i = 0;
	while (i < NB_N_HIDDEN2)
	{
		nw[2][i].f = &ft_sigmoid;
		nw[2][i].stop = false;
		i++;
	}
	nw[2][i].stop = false;
	nw[2][i + 1].stop = true;
	i = 0;
	while (i < NB_N_OUTPUT)
	{
		nw[3][i].f = &ft_sigmoid;
		nw[3][i].stop = false;
		i++;
	}
	nw[3][i].stop = true;
	return (nw);
}

t_neurone	**ft_link_layer(t_neurone **nw, int prev, int prev_size, int next_size) // Create links beetwen neurons of two layers
{
	int	i;
	int	j;

	i = 0;
	while (i < prev_size)
	{

		j = 0;
		if (!(nw[prev][i].weights = (float*)malloc(sizeof(float) * next_size))) // float *weights containing the weights of a single neuron
			return (NULL);
	//	printf("Neurone %d :\n", i);
		while (j < next_size)
		{
			nw[prev][i].weights[j] = random_dbl(-1, 1); // set every weight randomly
	//		printf("%f\n", nw[prev][i].weights[j]);
			j++;
		}
		i++;
	}
	return (nw);
}

t_neurone	**ft_address_mem(t_neurone **nw) // Address the network in memory into a t_neurone **network
{	
	if (!(nw = (t_neurone**)malloc(sizeof(t_neurone*) * (NB_LAYERS + 1))))
		return (NULL);
	if (!(nw[0] = (t_neurone*)malloc(sizeof(t_neurone) * (NB_N_INPUT + 2))))
		return (NULL);
	nw[0][NB_N_INPUT + 1].weights = NULL;
	if (!(nw[1] = (t_neurone*)malloc(sizeof(t_neurone) * (NB_N_HIDDEN1 + 2))))
		return (NULL);
	nw[1][NB_N_HIDDEN1 + 1].weights = NULL;
	nw = ft_link_layer(nw, 0, NB_N_INPUT + 1, NB_N_HIDDEN1);
	if (!(nw[2] = (t_neurone*)malloc(sizeof(t_neurone) * (NB_N_HIDDEN2 + 2))))
		return (NULL);
	nw[2][NB_N_HIDDEN2 + 1].weights = NULL;
	nw = ft_link_layer(nw, 1, NB_N_HIDDEN1 + 1, NB_N_HIDDEN2);
	if (!(nw[3] = (t_neurone*)malloc(sizeof(t_neurone) * (NB_N_OUTPUT + 1))))
		return (NULL);
	nw[3][NB_N_OUTPUT].weights = NULL;
	nw = ft_link_layer(nw, 2, NB_N_HIDDEN2 + 1, NB_N_OUTPUT);
	nw[4] = NULL;
	if (!(nw = ft_init_activations(nw)))
		return (NULL);
	return (nw);
}

t_neurone	**ft_init_network(void)
{
	t_neurone	**nw;
	int		i;

	i = 0;
	nw = NULL;
	if (!(nw = ft_address_mem(nw)))
		return (NULL);
	return (nw);
}
