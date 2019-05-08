#include "../includes/main.h"

int	main(int argc, char **argv)
{
	t_env	env;

	if (argc == 4 && ft_strcmp(argv[1], "--backprop") == 0)
		ft_train(&env, argv[2], argv[3]);
	else if (argc == 3 && ft_strcmp(argv[1], "--test") == 0)
		ft_test(&env, argv[2]);
	return (0);
}
