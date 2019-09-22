#include "../includes/main.h"

char	*ft_fill_pixel(char *map, t_point coord, int color)
{
	int		i;

	if (coord.y <= 0 || coord.x <= 0 || coord.y >= HEIGHT || coord.x >= WIDTH)
		return (map);
	i = ((round(coord.y) - 1) * WIDTH + round(coord.x)) * 4;
	if (map[i] != 0 && map[i + 1] != 0 && map[i + 2] != 0 && (color == 0XEEEEEE || color == 0XDDDDDD || color == 0XCCCCCC || color == 0XBBBBBB))
		return (map);
	map[i] = color >> 16 & 255;
	map[i + 1] = color >> 8 & 255;
	map[i + 2] = color & 255;
	return (map);
}

char	*ft_draw_square(char *map, t_point point, int size, int color)
{
	int 	xt = point.x;
	int 	yt = point.y;
	int 	i = 0;
	int 	j = 0;

	while (i < size)
	{
		map = ft_fill_pixel(map, point, color);
		map = ft_fill_pixel(map, point, color);
		point.x++;
		i++;
	}
	while (j < size)
	{
		map = ft_fill_pixel(map, point, color);
		map = ft_fill_pixel(map, point, color);
		point.y++;
		j++;
	}
	while (point.x > xt)
	{
		map = ft_fill_pixel(map, point, color);
		map = ft_fill_pixel(map, point, color);
		point.x--;
	}
	while (point.y > yt)
	{
		map = ft_fill_pixel(map, point, color);
		map = ft_fill_pixel(map, point, color);
		point.y--;
	}
	return (map);
}

char	*ft_draw_circle(char *img, t_point coord, float size, float pre, float post, int color)
{
	float 	y;
	float 	k;
 	float 	m = 2.0;
 	int		save[2] = {coord.x, coord.y};
 	
 	y = size;
	for (int i = -y; i <= y + 10; i++) 
	{
		for (int j = -y; j <= y; j++) 
 		{
 			k = ((i - 1 * m) / y) * ((i - 1 * m) / y) + (j / y) * (j/ y);
			if (k > pre && k < post)
 			{
 				coord.x += i;
 				coord.y += j;
 				ft_fill_pixel(img, coord, color);
 				coord.x = save[0];
 				coord.y = save[1];
 			}
 		}
 	}
 	return (img);
}

float 	ft_average(char *img, int i, int pos)
{
	float 	ret = 0;
	int 	j = 0;

	while (j < 4)
	{
		ret += ft_abs((int)img[i + pos]);
		ret += ft_abs((int)img[i + pos + 4]);
		ret += ft_abs((int)img[i + pos + 8]);
		ret += ft_abs((int)img[i + pos + 12]);
		i += 3200;
		j++;
	}
	return (ret);
}

float 	*ft_compute(t_env *env)
{
	float 	*input;
	int		average;
	int 	i = 0;
	int 	k = 0;
	int 	pos = ((241 - 1) * WIDTH + 341) * 4;

	if (!(input = (float*)malloc(sizeof(float) * 784)))
		return (NULL);
	if (!(env->mlx.out = (float*)malloc(sizeof(float) * 10)))
		return (NULL);
	while (k < 784)
	{
		average = ft_average(env->mlx.img, i, pos);
		input[k] = (float)((float)average / 16) * 1.45 / 100;
//		printf("Input[%d] = %f\n", k, input[k]);
		i += 16;
		if (k % 28 == 0 && k != 0)
		{
			input[k] = 0;
			i += 12352;
		}
		k++;
	}
	ft_fire(env->nw, input);
	i = 0;
	while (i < 10)
	{
//		printf("%d : %f\n", i, env->nw[3][i].output);
		env->mlx.out[i] = env->nw[3][i].output;
		i++;
	}
	free(input);
	return (env->mlx.out);
}

int 	ft_make_color(float out)
{
	int 	ret = 0;
	int 	tmp;

	tmp = out * 256;
	ret += tmp;
	ret += tmp * 256;
	ret += tmp * 65536;
	return (ret);
}

char	*ft_draw_base(char *img, float *out)
{
	t_point		coord;
	int 		i = 0;

	i = 0;
	coord.x = 340;
	coord.y = 240;
	img = ft_draw_square(img, coord, 113, 0XFFFFFF);
	coord.x--;
	coord.y--;
	img = ft_draw_square(img, coord, 115, 0XCCCCCC);
	coord.x--;
	coord.y--;
	img = ft_draw_square(img, coord, 117, 0X999999);
	coord.x--;
	coord.y--;
	img = ft_draw_square(img, coord, 119, 0X777777);
	coord.x = 700;
	coord.y = 80;
	while (i < 10)
	{
		img = ft_draw_circle(img, coord, 20.0, (!out) ? 0.80 : 0, (!out) ? 1.10 : 1, (!out) ? 0 : ft_make_color(out[i]));
		img = ft_draw_circle(img, coord, 20.0, 0.90, 1.0, 0XFFFFFF);
		img = ft_draw_circle(img, coord, 21.0, 0.90, 1.0, 0XBBBBBB);
		img = ft_draw_circle(img, coord, 22.0, 0.90, 1.0, 0X999999);
		img = ft_draw_circle(img, coord, 23.0, 0.90, 1.0, 0X666666);
		coord.y += 50;
		i++;
	}
	coord.x = 320;
	coord.y = 500;
	img = ft_draw_circle(img, coord, 40.0, 0, 1, 0XAAAAAA);
	coord.x += 150;
	img = ft_draw_circle(img, coord, 40.0, 0, 1, 0XAAAAAA);
	return (img);
}

