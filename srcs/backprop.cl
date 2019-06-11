# define NB_LAYERS 4
# define NB_N_INPUT 784
# define NB_N_HIDDEN1 16
# define NB_N_HIDDEN2 16
# define NB_N_OUTPUT 10
# define BIAS 1

////////// Typedefs //////////

typedef struct	s_win
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	char 		*img;
	float 		*input;
	float 		*out;
	bool		press;
	int 		x;
	int 		y;
	int 		bpp;
	int 		size_line;
	int 		endian;
}				t_win;

typedef	struct 	s_point
{
	int 		x;
	int 		y;
}				t_point;

typedef struct s_img
{
	int 		digit;
	float		*input;
	float		*output;
}				t_img;

typedef struct	s_neurone
{
	float		input;
	float		(*f)(float);
	float		output;
	float 		biais;
	float		*weights;
	bool		stop;
}				t_neurone;

typedef struct s_env
{
	t_neurone	**nw;
	t_img		*dataset;
	float		*conf;
	float 		*output;
	float 		*changes;
	float 		*target;
	float 		*next_target;
	float 		l_rate;
	t_win		mlx;
}			t_env;

////////// Firing //////////
float		ft_sigmoid(float x);
inline void		ft_fire(t_neurone **nw, float *input);
inline float	ft_get_sum(t_neurone **nw, int layer, int layer_size, int neurone);
inline t_neurone	**ft_fill_in(t_neurone **nw, float *input);

float		ft_sigmoid(float x)
{
     float exp_value;
     float return_value;

     /*** Exponential calculation ***/
     exp_value = exp((float) -x);

     /*** Final sigmoid value ***/
     return_value = 1 / (1 + exp_value);
     return (return_value);
}

inline t_neurone	**ft_fill_in(t_neurone **nw, float *input)
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

inline float	ft_get_sum(t_neurone **nw, int layer, int layer_size, int neurone)
{
	float 	sum = 0;
	int 	i = 0;

	while (i < layer_size)
	{
		sum += (nw[layer][i].output * nw[layer][i].weights[neurone]);
		i++;
	}
	return (sum);
}

inline void		ft_fire(t_neurone **nw, float *input)
{
	int 		i = 0;

	if (!(nw = ft_fill_in(nw, input)))
		return ;
	while (i < NB_N_HIDDEN1)
	{
		nw[1][i].input = ft_get_sum(nw, 0, NB_N_INPUT + 1, i);
		nw[1][i].output = ft_sigmoid(nw[1][i].input);
		i++;
	}
	i = 0;
	while (i < NB_N_HIDDEN2)
	{
		nw[2][i].input = ft_get_sum(nw, 1, NB_N_HIDDEN1 + 1, i);
		nw[2][i].output = ft_sigmoid(nw[2][i].input);
		i++;
	}
	i = 0;
	while (i < NB_N_OUTPUT)
	{
		nw[3][i].input = ft_get_sum(nw, 2, NB_N_HIDDEN2 + 1, i);
		nw[3][i].output = ft_sigmoid(nw[3][i].input);
		i++;
	}
}

////////// Gradient computation //////////

inline 	int 	ft_pos_weight(int layer, int i, int j);
t_env  	*ft_make_target(t_env *env, int layer, int index);
__global float	*ft_average_changes(__global float *changes, t_env *env, int index);
float 	ft_compute_grad(t_env *env, int layer, int i, int j);

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
	env->next_target = memset(env->next_target, 0, sizeof(float) * 16);
	return (env);
}

inline 	int 	ft_pos_weight(int layer, int i, int j)
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

__global float	*ft_average_changes(__global float *changes, t_env *env, int index)
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
				changes[ft_pos_weight(layer, i, j)] += ft_compute_grad(env, layer, i, j);
				j++;
			}
			i++;
		}
		layer--;
	}
	return ((__global float*)changes);
}

__kernel void	ft_gradient_descent(__global float *changes, t_env env)
{
	int		index = get_global_id(0);

	ft_fire(env.nw, env.dataset[index].input);
	changes = ft_average_changes(changes, &env, index);
}
