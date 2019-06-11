#include "../includes/main.h"

t_neurone	**ft_fill_in(t_neurone **nw, float *input)
{
	int 	i = 0;

	while (i < NB_N_INPUT)
	{
		nw[0][i].output = input[i];
		i++;
	}
	nw[0][i].output = BIAS;
	nw[1][NB_N_HIDDEN1].output = BIAS;
	nw[2][NB_N_HIDDEN2].output = BIAS;
	return (nw);
}

float		ft_get_sum(t_neurone **nw, int layer, int layer_size, int neurone)
{
	float 	sum = 0.000;
	int 	i = 0;

	while (i < layer_size)
	{
		sum += (nw[layer][i].output * nw[layer][i].weights[neurone]);
		i++;
	}
	return (sum);
}

void		ft_fire(t_neurone **nw, float *input)
{
	int 		i = 0;

	if (!(nw = ft_fill_in(nw, input)))
		return ;
	while (i < NB_N_HIDDEN1)
	{
		nw[1][i].input = ft_get_sum(nw, 0, NB_N_INPUT + 1, i);
		nw[1][i].output = nw[1][i].f(nw[1][i].input);
		i++;
	}
	i = 0;
	while (i < NB_N_HIDDEN2)
	{
		nw[2][i].input = ft_get_sum(nw, 1, NB_N_HIDDEN1 + 1, i);
		nw[2][i].output = nw[2][i].f(nw[2][i].input);
		i++;
	}
	i = 0;
	while (i < NB_N_OUTPUT)
	{
		nw[3][i].input = ft_get_sum(nw, 2, NB_N_HIDDEN2 + 1, i);
		nw[3][i].output = nw[3][i].f(nw[3][i].input);
		i++;
	}
}

t_neurone	**ft_apply_weights(t_neurone **network, float *weights)
{
	int 	i = 0;
	int 	layer = 0;
	int		j = 0;
	int 	k;

//	printf("------------ INPUT LAYER ------------------\n");
	while (j < NB_N_INPUT + 1)
	{
		k = 0;
//		printf("Layer %d -> Neurone %d :\n", layer, j);
		while (k < NB_N_HIDDEN1)
		{
			network[layer][j].weights[k] = weights[i];
//			printf("Poids %d = %f\n", k, network[layer][j].weights[k]);
			i++;
			k++;
		}
		j++;
	}
	j = 0;
	layer++;
	// printf("------------ HIDDEN LAYER 1 ------------------\n");
	while (j < NB_N_HIDDEN1 + 1)
	{
		// printf("Layer %d -> Neurone %d :\n", layer, j);
		k = 0;
		while (k < NB_N_HIDDEN2)
		{
			network[layer][j].weights[k] = weights[i];
 			// printf("Poids %d = %f\n", k, network[layer][j].weights[k]);
			i++;
			k++;
		}
		j++;
	}
	j = 0;
	layer++;
	// printf("------------ HIDDEN LAYER 2 ------------------\n");
	while (j < NB_N_HIDDEN2 + 1)
	{
		k = 0;
		// printf("Layer %d -> Neurone %d :\n", layer, j);
		while (k < NB_N_OUTPUT)
		{
			network[layer][j].weights[k] = weights[i];
			// printf("Poids %d = %f\n", k, network[layer][j].weights[k]);
			i++;
			k++;
		}
		j++;
	}
	return (network);
}
