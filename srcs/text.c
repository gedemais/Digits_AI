#include "../includes/main.h"

void	ft_draw_text(t_win *mlx)
{
	char	*str;
	int 	i = 0;
	int 	y = 80;

	while (i < 10)
	{
		mlx_string_put(mlx->mlx_ptr, mlx->win_ptr, 750, y - 12, 0XFFFFFF, (str = ft_itoa(i)));
		y += 50;
		ft_strdel(&str);
		i++;
	}
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr, 302, 488, 0XFFFFFF, "Wipe");
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr, 448, 488, 0XFFFFFF, "Guess");
}