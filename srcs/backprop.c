#include "../includes/main.h"

t_env 	*ft_setenv(t_env *env, char *digits, char *labels)
{
	int		i;

	i = 0;
	if (!(env->dataset = ft_make_dataset(digits, labels)))
		return (NULL);
	if (!(env->memory = (float*)malloc(sizeof(float) * 13002)))
		return (NULL);
	env->memory = ft_memset(env->memory, 0, sizeof(float) * 13002);
	if (!(env->changes = (float*)malloc(sizeof(float) * 13002)))
		return (NULL);
	if (!(env->conf = (float*)malloc(sizeof(float) * 13002)))
		return (NULL);
	if (!(env->target = (float*)malloc(sizeof(float) * 17)))
		return (NULL);
	if (!(env->next_target = (float*)malloc(sizeof(float) * 17)))
		return (NULL);
	return (env);
}

float 		*ft_apply_changes(float *conf, float *changes, float *memory, float l_rate)
{
	int 	i = 0;

	while (i < 13002)
	{
		conf[i] -= ((changes[i] + (memory[i] / 10)) / BATCH_SIZE) * l_rate;
		i++;
	}
	return (conf);
}

static inline 	int 	ft_pos_weight(int layer, int i, int j)
{
	int 	ret = 0;

	if (layer == 3)
	{
		ret = 12832;
		ret += j * 10;
		ret += i;
	}
	else if (layer == 2)
	{
		ret = 12560;
		ret += j * 16;
		ret += i;
	}
	else if (layer == 1)
	{
		ret += j * 16;
		ret += i;
	}
	return (ret);
}

float 			ft_cost(t_env *env, int index)
{
	float 		ret = 0;
	float 		tmp;
	int 		i = 0;

	while (i < 10)
	{
		tmp = env->nw[3][i].output - env->dataset[index].output[i];
		ret += (tmp * tmp); 
		i++;
	}
	return (ret);
}

float 	ft_compute_grad(t_env *env, int layer, int i, int j)
{
	float 	a;
	float 	dzw;
	float 	daz;
	float	dca;
	float 	pre_ret;

	a = env->nw[layer][i].output;
	dzw = env->nw[layer - 1][j].output;
	daz = a * (1 - a);
	dca = 2 * (a - env->target[i]);
	pre_ret = dzw * daz;
	if (layer > 1)
		env->next_target[i] += (pre_ret * env->nw[layer - 1][j].weights[i]);
	return (pre_ret * dca);
}

t_env  	*ft_make_target(t_env *env, int layer, int index)
{
	int 	i = 0;

	if (layer == 3)
	{
		while (i < 10)
		{
			env->target[i] = env->dataset[index].output[i];
			i++;
		}
	}
	else if (layer == 2)
	{
		while (i < 16)
		{
			env->target[i] -= (env->next_target[i] / 10);
			i++;
		}
	}
	else if (layer == 1)
	{
		while (i < 16)
		{
			env->target[i] -= (env->next_target[i] / 16);
			i++;
		}
	}
	env->next_target = ft_memset(env->next_target, 0, sizeof(float) * 16);
	return (env);
}

t_env	*ft_average_changes(t_env *env, int index)
{
	int 	layer = NB_LAYERS - 1;
	int 	i = 0;
	int 	j = 0;

	while (layer > 0)
	{
		i = 0;
		if (layer > 1)
			env = ft_make_target(env, layer, index);
		while (env->nw[layer][(i + (layer == 3 ? 0 : 1))].stop == false)
		{
			j = 0;
			while (env->nw[layer - 1][j].stop == false)
			{
				env->changes[ft_pos_weight(layer, i, j)] += ft_compute_grad(env, layer, i, j);
				j++;
			}
			i++;
		}
		layer--;
	}
	return (env);
}

t_env	*ft_gradient_descent(t_env *env, float *cout, int epoch)
{
	static int		index = 0;
	int 			i = 0;
	float 			cost = 0;

	env->nw = ft_apply_weights(env->nw, env->conf);
	while (i < BATCH_SIZE)
	{
		ft_fire(env->nw, env->dataset[index].input);
		env = ft_average_changes(env, index);
		cost += ft_cost(env, index);
		i++;
		index++;
		if (index == 60000)
			index = 0;
	}
	*cout = cost;
	if (cost < 10000 && epoch % 10 == 0)
		ft_export_weights(env->conf); // Saving the weights if a really good config is found
	return (env);
}

int		ft_backpropagation(t_env *env, char *digits, char *labels)
{
	float 	cost = 1000;
	float	prev_cost = 0;
	float 	average = 100000;
	float 	av = 100000;
	int 	epoch = 0;

	if (!(env = ft_setenv(env, digits, labels))) // Loading everything
		return (-1);
	env = ft_load_weights(env, "best"); // Setting the basic weights config to random
	env->l_rate = atof(ft_read(open("Learning_rate", O_RDONLY)));
	while (MAX_EPOCHS < 10000000) // Main Backpropagation loop
	{
		env->changes = ft_memset(env->changes, 0, sizeof(float) * 13002); // Flushing negative gradients table to 0
		env = ft_gradient_descent(env, &cost, epoch); // Computing the negative gradients table
		prev_cost = cost;
		printf("----- Epoch %d -----\nAverage = %.20f\nLearning_rate : %f\n", epoch, av, env->l_rate);
		printf("Learning Rate = %f\n", env->l_rate);
		printf("Cost = %f\n", cost);
		if (epoch % 1000 != 0) // Average of the cost on the last thousand epochs
			average += cost;
		else
		{
			env->l_rate = atof(ft_read(open("Learning_rate", O_RDONLY)));
			av = average / BATCH_SIZE - 1;
			average = 0;
		}
		if (av < 10)
			exit (EXIT_SUCCESS);
		env->conf = ft_apply_changes(env->conf, env->changes, env->memory, env->l_rate);
		env->memory = ft_memcpy(env->memory, env->changes, sizeof(float) * 13002);
		epoch++;
	}
	return (0);	
}
