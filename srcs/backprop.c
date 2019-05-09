#include "../includes/main.h"

t_env 	*ft_setenv(t_env *env, char *digits, char *labels)
{
	if (!(env->dataset = ft_make_dataset(digits, labels)))
		return (NULL);
	if (!(env->changes = (double*)malloc(sizeof(double) * 13002)))
		return (NULL);
	if (!(env->conf = (double*)malloc(sizeof(double) * 13002)))
		return (NULL);
	if (!(env->target = (double*)malloc(sizeof(double) * 17)))
		return (NULL);
	if (!(env->next_target = (double*)malloc(sizeof(double) * 17)))
		return (NULL);
	return (env);
}

double 		*ft_apply_changes(double *conf, double *changes, double l_rate)
{
	int 	i = 0;

	while (i < 13002)
	{
		conf[i] -= (changes[i] / BATCH_SIZE) * l_rate;
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

double 			ft_cost(t_env *env, int index)
{
	double 		ret = 0;
	double 		tmp;
	int 		i = 0;

	while (i < 10)
	{
		tmp = env->nw[3][i].output - env->dataset[index].output[i];
		ret += (tmp * tmp); 
		i++;
	}
	return (ret);
}

double 	ft_compute_grad(t_env *env, int layer, int i, int j)
{
	double 	a;
	double 	dzw;
	double 	daz;
	double	dca;
	double 	pre_ret;

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
	env->next_target = ft_memset(env->next_target, 0, sizeof(double) * 16);
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

t_env	*ft_gradient_descent(t_env *env, double *cout, int epoch)
{
	int 			i = 0;
	double 			cost = 0;

	env->nw = ft_apply_weights(env->nw, env->conf);
	while (i < BATCH_SIZE)
	{
		ft_fire(env->nw, env->dataset[i].input);
		env = ft_average_changes(env, i);
		cost += ft_cost(env, i);
		i++;
	}
	*cout = cost;
	if (cost < 10000 && epoch % 10 == 0)
		ft_export_weights(env->conf); // Saving the weights if a really good config is found
	return (env);
}

double	ft_get_cost(t_env *env, double *conf)
{
	double		cost = 0;
	int 		i;

	i = 0;
	env->nw = ft_apply_weights(env->nw, conf);
	while (i < BATCH_SIZE)
	{
		ft_fire(env->nw, env->dataset[i].input);
		cost += ft_cost(env, i);
		i++;
	}
	return (cost);
}

double	ft_guess_lrate(t_env *env, double cost)
{
	double		*a;
	double		da = cost + 1;
	double		try;
	int			trys = 0;

	if (!(a = (double*)malloc(sizeof(double) * 13002)))
		return (-1);
	env->nw = ft_apply_weights(env->nw, env->conf);
//	ft_putstr("Exploring");
	while (da > cost)
	{
		try = random_dbl(0, 5);
		a = ft_cpy_weights(a, env->conf);
		a = ft_apply_changes(a, env->changes, try);
		da = ft_get_cost(env, a);
	//	printf("Try %d : %f\n", trys, da);
		trys++;
	}
	ft_putchar('\n');
	return (try);
}

int		ft_backpropagation(t_env *env, char *digits, char *labels)
{
	double 	cost = 1000;
	double	prev_cost = 0;
	double 	average = 100000;
	double 	av = 100000;
	int 	epoch = 0;

	if (!(env = ft_setenv(env, digits, labels))) // Loading everything
		return (-1);
	env = ft_load_weights(env, "bestever"); // Setting the basic weights config to random
//	env->conf = ft_set_rand(env->conf, -5, 5);
	env->l_rate = atof(ft_read(open("Learning_rate", O_RDONLY)));
	while (MAX_EPOCHS < 10000000) // Main Backpropagation loop
	{
		env->changes = ft_memset(env->changes, 0, sizeof(double) * 13002); // Flushing negative gradients table to 0
		env = ft_gradient_descent(env, &cost, epoch); // Computing the negative gradients table
		prev_cost = cost;
		printf("----- Epoch %d -----\nAverage = %.20f\nLearning_rate : %f\n", epoch, av, env->l_rate);
		printf("Learning Rate = %f\n", env->l_rate);
		printf("Cost = %f\n", cost);
		if (epoch % 50 != 0) // Average of the cost on the last thousand epochs
			average += cost;
		else
		{
			env->l_rate = atof(ft_read(open("Learning_rate", O_RDONLY)));
			av = average / 9;
			average = 0;
		}
		if (av < 10)
			exit (EXIT_SUCCESS);
		env->conf = ft_apply_changes(env->conf, env->changes, env->l_rate);
		epoch++;
	}
	return (0);	
}
