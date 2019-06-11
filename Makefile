# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gedemais <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/05 00:59:17 by gedemais          #+#    #+#              #
#    Updated: 2019/06/11 06:34:15 by gedemais         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEF = \033[0m
GRA = \033[1m
SOU = \033[4m
BLI = \033[5m
BLA = \033[30m
RED = \033[31m
GRE = \033[32m
YEL = \033[33m
BLU = \033[34m
PUR = \033[35m
CYA = \033[36m
WHI = \033[37m
ORG = \033[1;31m

NAME = digits_AI

CC = @gcc -O1 -O2 -O3 -Ofast -march=native

FLAGS = -g -fsanitize=address

CFLAGS = -Wall -Werror -Wextra

LIB_PATH = libft/

LIB = libft/libft.a

SRCS_NAME = main.c\
			fts.c\
			gen.c\
			mlx.c\
			test.c\
			train.c\
			text.c\
			draw_pixel.c\
			backprop.c\
			get_dataset.c\
			apply_weights.c\
			opencl.c\
			init_network.c

SRCS_PATH = srcs/

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_NAME = $(SRCS_NAME:.c=.o)

OBJS_PATH = obj/

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

all : $(LIB) $(NAME)

$(NAME) : $(LIB) $(SRCS) includes/main.h	
		@echo "Making $(CYA)Digits_AI$(DEF)..."
		$(CC) $(CFLAGS) -o $(NAME) -I /usr/local/include/ -I includes/ -I libft/ $(SRCS) -L /usr/local/lib/ -lmlx -framework OpenGL -framework OpenCL -framework AppKit $(LIB)
		@echo "$(GRE)Done !$(DEF)"

$(LIB) : $(LIB_PATH)
		@echo "Making $(RED)Libft$(DEF)..."
		@make re -C $(LIB_PATH)

fsanitize : $(SRCS)
	gcc -g3 -fsanitize=address $(SRCS) $(LIB)

clean : 
	@rm -rf obj/
	@make clean -C $(LIB_PATH)

fclean : clean
	@make fclean -C $(LIB_PATH)
	@rm -rf $(NAME)

re : fclean all
