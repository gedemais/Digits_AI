#ifndef MAIN_H
# define MAIN_H

# include "../libft/libft.h"
# include "mlx.h"
# include <stdio.h>
# include <dirent.h>
# include <sys/types.h>
# include <fcntl.h>
# include <math.h>
# include <pthread.h>
# include <stdbool.h>

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
# define BATCH_SIZE 60000

# define HEIGHT 600
# define WIDTH 800
# define MLX ((t_win*)(param))
# define ENV ((t_env*)(param))

typedef struct	s_win
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	char 		*img;
	double 		*input;
	double 		*out;
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
	double		*input;
	double		*output;
}				t_img;

typedef struct	s_neurone
{
	double		input;
	double		(*f)(double);
	double		output;
	double 		biais;
	double		*weights;
	bool		stop;

}				t_neurone;

typedef struct s_env
{
	t_neurone	**nw;
	t_img		*dataset;
	double		*conf;
	double 		*output;
	double 		*changes;
	double 		*target;
	double 		*next_target;
	double 		l_rate;
	t_win		mlx;
}				t_env;

char		*ft_fill_pixel(char *map, t_point coord, int color);
char		*ft_draw_square(char *map, t_point point, int size, int color);
char		*ft_draw_base(char *img, double *out);
char		*ft_draw_circle(char *img, t_point coord, double size, double pre, double post, int color);
char		*ft_draw_pixel(char *img, t_point point);
void		ft_draw_text(t_win *mlx);
double 		*ft_compute(t_env *env);

t_env		*ft_load_weights(t_env *env, char *file_name);
int 		ft_train(t_env *env, char *digits, char *labels);
int 		ft_test(t_env *env, char *file_name);
int			ft_backpropagation(t_env *env, char *digits, char *labels);

double 		ft_cost(t_env *env, int index);
double		random_dbl(double min, double max);
double		ft_sigmoid(double x);

t_img 		*ft_make_dataset(char *imgs, char *labels);
char		*ft_read(int fd);

void		ft_fire(t_neurone **nw, double *input);
int 		ft_export_weights(double *weights);
double 		*ft_set_rand(double *weights, double min, double max);

t_neurone	**ft_init_network(void);
t_neurone	**ft_fill_in(t_neurone **nw, double *input);
t_neurone	**ft_apply_weights(t_neurone **network, double *weights);
double 		*ft_cpy_weights(double *dest, double *src);

#endif
