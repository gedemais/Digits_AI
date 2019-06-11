#include "../includes/main.h"

void	ft_loading_bar(int size)
{
	int 	taux;
	int 	i = 0;

	taux = size / 1000000;
	write(STDOUT_FILENO, "\e[1;1H\e[2J", 11);
	printf("Loading Data set : %d %%\n", taux);
	ft_putchar('[');
	while (i < taux)
	{
		ft_putstr("\033[01;32m#\033[0m");
		i++;
	}
	while (i < 100)
	{
		ft_putchar(' ');
		i++;
	}
	ft_putchar(']');
	ft_putchar('\n');
}

char	*ft_read(int fd)
{
	char	buff[BUFF_READ + 1];
	char	*dest;
	int		ret;
	int		size;
	int		i;

	i = 0;
	size = 0;
	if (!(dest = ft_strnew(0)))
		return (NULL);
	while ((ret = read(fd, buff, BUFF_READ)) > 0)
	{
		size += ret;
		ft_loading_bar(size);
		if (ret == -1 || !(dest = ft_strrealloc(dest, size)))
			return (NULL);
		buff[ret] = '\0';
		dest = ft_strcat(dest, buff);
		i++;
	}
	return (dest);
}

int		ft_is_pixel(char *base, char c) // Check if the current address is a white pixel into the file string
{
	int		i = 0;

	while (i < 16)
	{
		if (c == base[i])
			return (1);
		i++;
	}
	return (0);
}

float	*ft_read_img(char *file, char *base, int *jj)
{
	float 	*input;
	char 	buff[3];
	int 	j;
	int 	i = 0;

	j = *jj;
	if (!(input = (float*)malloc(sizeof(float) * IMG_SIZE)))
		return (NULL);
	while (file[j] != '\n' || file[j] == '=')
		j++;
	j++;
	while (file[j] != '=' && file[j + 1] != '=')
	{
		if (file[j] == ' ')
		{
			input[i] = 0.000000;
			i++;
		}
		else if (ft_is_pixel(base, file[j]) == 1)
		{
			buff[0] = file[j];
			buff[1] = file[j + 1];
			buff[2] = '\0';
			input[i] = (float)((float)atoi_base(buff, base) / 272);
			i++;
		}
		j += 2;
	}
	j++;
	j += 57;
	*jj = j;
	return (input);
}

void 	ft_print_dtab(float *output)
{
	int 	i = 0;

	while (i < 10)
	{
		printf("%f ", output[i]);
		i++;
	}
	printf("\n");
}

float	*ft_make_output(int digit)
{
	float 	*output;
	int 	i;

	i = 0;
	if (!(output = (float*)malloc(sizeof(float) * 10)))
		return (NULL);
	while (i < 10)
	{
		if (i == digit)
			output[i] = 1.000;
		else
			output[i] = 0.000;
		i++;
	}
	return (output);
}

t_img	*ft_get_img(int *labels, int fd) // Load the images into a t_img *tab of 60000 boxes
{
	t_img 	*dest;
	char	*base;
	char	*file;
	int 	i = 0;
	int 	j = 0;

	if (!(base = ft_strdup("0123456789ABCDEF")))
		return (NULL);
	if (!(dest = (t_img*)malloc(sizeof(t_img) * (DATASET_SIZE + 1))))
		return (NULL);
	if (!(file = ft_read(fd)))
		return (NULL);
	ft_putstr("Decompressing images...\n");
//	ft_putstr(file);
	while (i < DATASET_SIZE)
	{
		if (!(dest[i].input = ft_read_img(file, base, &j)))
			return (NULL);
		dest[i].digit = labels[i];
		if (!(dest[i].output = ft_make_output(labels[i])))
			return (NULL);
		i++;
	}
	ft_strdel(&file);
	return (dest);
}

int		*ft_get_digits(int fd) // Load an int *dest of the 60000 corresponding labels
{
	int		*dest;
	char	*input;
	int		i;

	i = 0;
	if (!(dest = (int*)malloc(sizeof(int) * 60001)))
		return (dest);
	while (get_next_line(fd, &input) > 0)
	{
		dest[i] = ft_atoi(input);
		i++;
		ft_strdel(&input);
	}
	dest[i] = -1;
	return (dest);
}

t_img 	*ft_make_dataset(char *imgs, char *labels) // Create the dataset by parsing labels and digits files
{
	t_img	*dataset;
	int		*digits;
	int		fd_imgs;
	int		fd_labels;

	fd_imgs = open(imgs, O_RDONLY);
	fd_labels = open(labels, O_RDONLY);
	dataset = NULL;
	digits = NULL;
	if (!(digits = ft_get_digits(fd_labels)))
		return (NULL);
	if (!(dataset = ft_get_img(digits, fd_imgs)))
		return (NULL);
	return (dataset);
}
