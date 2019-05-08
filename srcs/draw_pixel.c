#include "../includes/main.h"

char	*ft_draw_pixel(char *img, t_point point)
{
	img = ft_draw_circle(img, point, 6, 0, 1.3, 0XCCCCCC);
	img = ft_draw_circle(img, point, 5, 0, 1.3, 0XCCCCCC);
	img = ft_draw_circle(img, point, 4, 0, 1.3, 0XDDDDDD);
	img = ft_draw_circle(img, point, 3, 0, 1.3, 0XFEFEFE);
	img = ft_draw_circle(img, point, 2, 0, 1.3, 0XFFFFFF);
	img = ft_draw_circle(img, point, 1, 0, 1.3, 0XFFFFFF);
	point.x += 2;
	img = ft_fill_pixel(img, point, 0XFFFFFF);
	return (img);
}

