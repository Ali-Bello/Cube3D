NAME = cub3d

CFLAGS = -Wall -Werror -Wextra #-fsanitize=address -g3

MLX_FLAGS = -lXext -lX11 -lm

SRCS = srcs/debugging.c srcs/raycasting.c srcs/events.c srcs/render.c srcs/main.c

OBJS = $(SRCS:.c=.o)

MLX_LIB = libmlx_Linux.a

all : $(NAME)

$(NAME) : $(OBJS)
	cc $(CFLAGS) $(OBJS) $(MLX_LIB) $(MLX_FLAGS) -o $(NAME)

srcs/%.o : srcs/%.c Makefile includes/cub3d.h
	cc $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all