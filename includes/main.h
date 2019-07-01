#ifndef MAIN_H
# define MAIN_H

# include "../libft/libft.h"
# include "mlx.h"
# include <stdio.h>
# include <dirent.h>
# include <sys/types.h>
# include <fcntl.h>
# include <math.h>
# include <string.h>
# include <pthread.h>
# include <stdbool.h>
# include "/System/Library/Frameworks/OpenCL.framework/Versions/A/Headers/cl.h"

# define PRM ((t_env*)param)->test

# define MAX_EPOCHS 1000000
# define NB_LAYERS 4
# define NB_N_INPUT 784
# define NB_N_HIDDEN1 16
# define NB_N_HIDDEN2 16
# define NB_N_OUTPUT 10
# define DATASET_SIZE 60000
# define IMG_SIZE 784
# define BIAS 1
# define BUFF_READ 4194304
# define BUFF_RAND 8388608
# define BATCH_SIZE 100

# define HEIGHT 600
# define WIDTH 800
# define MLX ((t_win*)(param))
# define ENV ((t_env*)(param))

typedef struct			s_opencl
{

	cl_command_queue	queue;
	cl_device_id		device;
	cl_context			context;
	cl_program			program;
	cl_kernel			kernel;
	cl_mem				buffer;
}						t_opencl;

typedef struct	s_win
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	char 		*img;
	float 		*input;
	float 		*out;
	bool		press;
	int 		x;
	int 		y;
	int 		bpp;
	int 		size_line;
	int 		endian;
}				t_win;

typedef	struct 	s_point
{
	int 		x;
	int 		y;
}				t_point;

typedef struct s_img
{
	int 		digit;
	float		*input;
	float		*output;
}				t_img;

typedef struct	s_neurone
{
	float		input;
	float		(*f)(float);
	float		output;
	float 		biais;
	float		*weights;
	bool		stop;
}				t_neurone;

typedef struct s_env
{
	t_neurone	**nw;
	t_img		*dataset;
	float		*conf;
	float		*memory;
	float 		*output;
	float 		*changes;
	float 		*target;
	float 		*next_target;
	float 		l_rate;
	t_win		mlx;
	t_opencl	s;
}				t_env;

char		*ft_fill_pixel(char *map, t_point coord, int color);
char		*ft_draw_square(char *map, t_point point, int size, int color);
char		*ft_draw_base(char *img, float *out);
char		*ft_draw_circle(char *img, t_point coord, float size, float pre, float post, int color);
char		*ft_draw_pixel(char *img, t_point point);
void		ft_draw_text(t_win *mlx);
float 		*ft_compute(t_env *env);

t_env		*ft_load_weights(t_env *env, char *file_name);
int 		ft_train(t_env *env, char *digits, char *labels);
int 		ft_test(t_env *env, char *file_name);
int			ft_backpropagation(t_env *env, char *digits, char *labels);

float 		ft_cost(t_env *env, int index);
float		random_dbl(float min, float max);
float		ft_sigmoid(float x);

t_img 		*ft_make_dataset(char *imgs, char *labels);
char		*ft_read(int fd);

void		ft_fire(t_neurone **nw, float *input);
int 		ft_export_weights(float *weights);
float 		*ft_set_rand(float *weights, float min, float max);

t_neurone	**ft_init_network(void);
t_neurone	**ft_fill_in(t_neurone **nw, float *input);
t_neurone	**ft_apply_weights(t_neurone **network, float *weights);
float 		*ft_cpy_weights(float *dest, float *src);

int			ft_init_opencl(t_opencl *s);
float		*ft_run_kernel(t_env env, t_opencl *s, float *changes);
#endif
