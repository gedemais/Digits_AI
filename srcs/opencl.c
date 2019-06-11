/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gedemais <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 20:45:28 by gedemais          #+#    #+#             */
/*   Updated: 2019/06/11 07:41:35 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

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

int		ft_init_opencl(t_opencl *s)
{
	char	*source;
	int		fd;
	cl_int	ret;

	if ((fd = open("srcs/backprop.cl", O_RDONLY)) == -1)
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
	s->kernel = clCreateKernel(s->program, "ft_gradient_descent", &ret);
	if (ret != CL_SUCCESS)
		return (ft_opencl_exit(3));
	s->buffer = clCreateBuffer(s->context, CL_MEM_WRITE_ONLY, (sizeof(float) * 13002), NULL, NULL);
	ft_strdel(&source);
	return (0);
}

float	*ft_run_kernel(t_env env, t_opencl *s, float *changes)
{
	int		i;

	i = 0;
	clSetKernelArg(s->kernel, 0, sizeof(cl_mem), (void*)&s->buffer);
	clSetKernelArg(s->kernel, 1, sizeof(t_env), (void*)&env);
	clEnqueueNDRangeKernel(s->queue, s->kernel, 1, NULL, (size_t[]){60000},
		NULL, 0, NULL, NULL);
	if (clEnqueueReadBuffer(s->queue, s->buffer, CL_FALSE, 0, sizeof(float) * 13002,
			changes, 0, NULL, NULL) != CL_SUCCESS)
	{
		ft_putstr_fd("Copy from GPU failed\n", 2);
		return (NULL);
	}
	if (clFinish(s->queue) != CL_SUCCESS)
	{
		ft_putstr_fd("Queue waiting failed\n", 2);
		return (NULL);
	}
	return (changes);
}
