#include "../includes/main.h"

int 	ft_exit(void *param)
{
	(void)param;
	exit(EXIT_SUCCESS);
}

void	reframe(void *param)
{
//	mlx_clear_window(MLX->mlx_ptr, MLX->win_ptr);
//	printf("Reframe\nx = %d\ny = %d\nPress = %d\n", MLX->x, MLX->y, MLX->press);
	mlx_put_image_to_window(ENV->mlx.mlx_ptr, ENV->mlx.win_ptr, ENV->mlx.img_ptr, 0, 0);
	ft_draw_text(&(ENV->mlx));
}

t_env	*ft_load_weights(t_env *env, char *file_name)
{
	char	*file;
	int 	fd;
	int 	i = 0;
	int 	j = 0;

	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(" : No such file or directory\n", 2);
		return (NULL);
	}
	file = ft_read(fd);
	while (i < 13002)
	{
		env->conf[i] = atof(&file[j]);
		while(file[j] != '\n')
			j++;
		j++;
		i++;
	}
	return (env);
}

t_env 	*ft_init_test(t_env *env, char *file_name)
{
	if (!(env->conf = (double*)malloc(sizeof(double) * 13002)))
		return (NULL);
	if (!(env->output = (double*)malloc(sizeof(double) * 10)))
		return (NULL);
	if (!(env->nw = ft_init_network()))
		return (NULL);
	if (!(env = ft_load_weights(env, file_name)))
		return (NULL);
	ft_apply_weights(env->nw, env->conf);
	return (env);
}

int 	ft_is_in(t_point point)
{
	if (point.x > 340 && point.x < 448 && point.y > 240 && point.y < 348)
		return (1);
	return (0);
}

char	*ft_wipe(char *img)
{
	t_point	coord;

	coord.x = 341;
	while (coord.x < 454)
	{
		coord.y = 240;
		while (coord.y < 354)
		{
			ft_fill_pixel(img, coord, 0x000000);
			coord.y++;
		}
		coord.x++;
	}
	return (img);
}

int 	ft_mouse_event(void *param)
{
	t_point		point;

	if (ENV->mlx.press == false)
		return (0);
	ENV->mlx.out = NULL;
	point.x = ENV->mlx.x;
	point.y = ENV->mlx.y;
	if (ft_is_in(point) == 1 && ENV->mlx.press == true)
		ENV->mlx.img = ft_draw_pixel(ENV->mlx.img, point);
	else if (point.x > 283 && point.x < 364 && point.y > 461 && point.y < 541 && MLX->press == true)
		ENV->mlx.img = ft_wipe(ENV->mlx.img);
	else if (point.x > 433 && point.x < 514 && point.y > 461 && point.y < 541)	
		ENV->mlx.out = ft_compute(ENV);
	ENV->mlx.img = ft_draw_base(ENV->mlx.img, ENV->mlx.out);
	if (ENV->mlx.out)
	{
		free(ENV->mlx.out);
		ENV->mlx.out = NULL;
	}
	reframe(param);
	return (1);
}

int 	ft_press(int x, int y, int button, void *param)
{
	ENV->mlx.x = x;
	ENV->mlx.y = y;
	(void)button;
	ENV->mlx.press = true;
	return (1);
}

int 	ft_stop(int x, int y, int button, void *param)
{
	ENV->mlx.x = x;
	ENV->mlx.y = y;
	(void)button;
	ENV->mlx.press = false;
	return (1);
}

int 	ft_pos(int x, int y, void *param)
{
	if (ENV->mlx.press)
	{
		ENV->mlx.x = x;
		ENV->mlx.y = y;
		ft_mouse_event(param);
	}
//	reframe(param);
	return (1);
}

int 	ft_test(t_env *env, char *file_name)
{
	t_point		coords;

	coords.x = 120;
	coords.y = 220;
	if (!(env = ft_init_test(env, file_name)))
		return (-1);
	env->mlx.mlx_ptr = mlx_init();
	env->mlx.win_ptr = mlx_new_window(env->mlx.mlx_ptr, WIDTH, HEIGHT, "Digits AI Visual");
	env->mlx.img_ptr = mlx_new_image(env->mlx.mlx_ptr, WIDTH, HEIGHT);
	env->mlx.img = mlx_get_data_addr(env->mlx.img_ptr, &(env->mlx.bpp), &(env->mlx.size_line), &(env->mlx.endian));
	env->mlx.img = ft_draw_base(env->mlx.img, NULL);
	mlx_put_image_to_window(env->mlx.mlx_ptr, env->mlx.win_ptr, env->mlx.img_ptr, 0, 0);
	ft_draw_text(&env->mlx);

//	mlx_mouse_hook(env->mlx.win_ptr, ft_mouse_event, &env->mlx);
	env->mlx.press = false;
	env->mlx.x = 0;
	env->mlx.y = 0;
	mlx_hook(env->mlx.win_ptr, 4, (1L<<2), ft_press, env);
	mlx_hook(env->mlx.win_ptr, 5, (1L<<3), ft_stop, env);
	mlx_hook(env->mlx.win_ptr, 6, 0, ft_pos, env);
	mlx_hook(env->mlx.win_ptr, 17, 1L << 17, ft_exit, env);
//	mlx_loop_hook(env->mlx.win_ptr, ft_mouse_event, &env->mlx);
	mlx_loop(env->mlx.mlx_ptr);
	return (0);
}
