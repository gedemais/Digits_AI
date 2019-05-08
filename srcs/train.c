#include "../includes/main.h"

int		ft_init_train(t_env *env)
{
	if (!(env->nw = ft_init_network()))
		return (-1);
	return (0);
}

int 	ft_train(t_env *env, char *digits, char *labels)
{
	if (ft_init_train(env) == -1)
	{
		ft_putstr("ft_init_train failed\n");
		return (-1);
	}
	if (ft_backpropagation(env, digits, labels) == -1)
	{
		ft_putstr("ft_backpropagation failed\n");
		return (-1);
	}
	return (0);
}
