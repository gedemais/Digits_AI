#include "../includes/main.h"

double		ft_sigmoid(double x)
{
     double exp_value;
     double return_value;

     /*** Exponential calculation ***/
     exp_value = exp((double) -x);

     /*** Final sigmoid value ***/
     return_value = 1 / (1 + exp_value);
     return (return_value);
}
/*
double	random_dbl(double min, double max)
{
	static char		buff[BUFF_RAND + 1];
	static int 		index = 0;
	static int 			fd = 0;
	double 			percent;
	double 			ret;

	if (fd == 0)
		fd = open("/dev/random", O_RDONLY);
	if (index > BUFF_RAND - 2)
		index = 0;
	if (index == 0)
	{
		if (read(fd, buff, BUFF_RAND) == -1)
			return (0);
		buff[BUFF_RAND] = '\0';
	}
	ret = (double)((double)buff[index] * (double)buff[index + 1]);
	percent = ret / 16129;
	ret = ((max - min) * percent);
	index += 2;
	return (ret);
}
*/
double    random_dbl(double min, double max)
{
   double range = (max - min);
   double div = RAND_MAX / range;
   return min + (rand() / div);
}

int 	ft_export_weights(double *weights)
{
	int 	i = 0;
	int 	fd;

	fd = open("best", O_CREAT | O_WRONLY, 777);
	while (i < 13002)
	{
		dprintf(fd, "%.10f\n", weights[i]);
		i++;
	}
	close(fd);
	system ("chmod 777 best");
	return (0);
}
