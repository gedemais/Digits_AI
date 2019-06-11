#include "../includes/main.h"

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
/*
float	random_dbl(float min, float max)
{
	static char		buff[BUFF_RAND + 1];
	static int 		index = 0;
	static int 			fd = 0;
	float 			percent;
	float 			ret;

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
	ret = (float)((float)buff[index] * (float)buff[index + 1]);
	percent = ret / 16129;
	ret = ((max - min) * percent);
	index += 2;
	return (ret);
}
*/
float    random_dbl(float min, float max)
{
   float range = (max - min);
   float div = RAND_MAX / range;
   return min + (rand() / div);
}

int 	ft_export_weights(float *weights)
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
