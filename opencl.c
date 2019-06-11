/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gedemais <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 20:45:28 by gedemais          #+#    #+#             */
/*   Updated: 2019/06/10 22:29:29 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

int		ft_opencl_exit(int error)
{
	if (error == 1)
		ft_putstr_fd("Identification failed\n", 2);
	else if (error == 2)
		ft_putstr_fd("Compilation failed\n", 2);
	else if (error == 3)
		ft_putstr_fd("Failed to create kernel\n", 2);
	return (-1);
}

void	ft_set_kernel_args(t_mlx *env, t_opencl *s)
{
	int			p;
	int			wdt;
	int			hgt;
	float		juliax;
	float		juliay;

	p = env->psychedelic ? 1 : 0;
	wdt = WDT;
	hgt = HGT;
	juliax = (float)*julia_x();
	juliay = (float)*julia_y();
	clSetKernelArg(s->kernel, 0, sizeof(cl_mem), (void*)&s->buffer);
	clSetKernelArg(s->kernel, 1, sizeof(int), (void*)&wdt);
	clSetKernelArg(s->kernel, 2, sizeof(int), (void*)&hgt);
	clSetKernelArg(s->kernel, 3, sizeof(t_fract), (void*)&env->draw);
	clSetKernelArg(s->kernel, 4, sizeof(int), (void*)&env->draw.palette);
	clSetKernelArg(s->kernel, 5, sizeof(int), (void*)&p);
	clSetKernelArg(s->kernel, 6, sizeof(int), (void*)&env->draw.mask);
	clSetKernelArg(s->kernel, 7, sizeof(float), (void*)&juliax);
	clSetKernelArg(s->kernel, 8, sizeof(float), (void*)&juliay);
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
		if (ret == -1 || !(dest = ft_strrealloc(dest, size)))
			return (NULL);
		buff[ret] = '\0';
		dest = ft_strcat(dest, buff);
		i++;
	}
	return (dest);
}

int		ft_init_opencl(t_opencl *s, char *img)
{
	char	*source;
	int		fd;
	cl_int	ret;

	if ((fd = open("srcs/fractol.cl", O_RDONLY)) == -1)
		return (-1);
	if (!(source = ft_read(fd)))
		return (-1);
	if (clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &(s->device), NULL)
		!= CL_SUCCESS)
		return (ft_opencl_exit(1));
	s->context = clCreateContext(NULL, 1, &(s->device), NULL, NULL, NULL);
	s->queue = clCreateCommandQueue(s->context, s->device, 0, NULL);
	s->program = clCreateProgramWithSource(s->context, 1, (const char**)&source,
		NULL, NULL);
	if (clBuildProgram(s->program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
		return (ft_opencl_exit(2));
	s->kernel = clCreateKernel(s->program, "fractol", &ret);
	if (ret != CL_SUCCESS)
		return (ft_opencl_exit(3));
	s->buffer = clCreateBuffer(s->context, CL_MEM_WRITE_ONLY,
		ft_screen_size(), NULL, NULL);
	ft_strdel(&source);
	return (0);
}

char	*ft_run_kernel(t_mlx *env, t_opencl *s, char *img)
{
	ft_set_kernel_args(env, s);
	clEnqueueNDRangeKernel(s->queue, s->kernel, 2, NULL, (size_t[]){HGT, WDT},
		NULL, 0, NULL, NULL);
	if (clEnqueueReadBuffer(s->queue, s->buffer, CL_FALSE, 0, ft_screen_size(),
			img, 0, NULL, NULL) != CL_SUCCESS)
	{
		ft_putstr_fd("Copy from GPU failed\n", 2);
		return (NULL);
	}
	if (clFinish(s->queue) != CL_SUCCESS)
	{
		ft_putstr_fd("Queue waiting failed\n", 2);
		return (NULL);
	}
	return (img);
}
