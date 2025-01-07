ERASE		:=  \033[2K\r
BLUE		:=	\033[34m
YELLOW		:=	\033[33m
GREEN		:=	\033[32m
MAGENTA		:=	\033[35m
CYAN		:=	\033[36m
BOLD		:=	\033[1m
END			:=	\033[0m

NAME = cub3d

CFLAGS = -Wall -Werror -Wextra -fsanitize=address -g3 -O3

MLX_FLAGS = -lXext -lX11 -lm

SRCS = debugging.c raycasting.c events.c render.c init.c main.c

OBJS_DIR = objs

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

MLX_LIB = libmlx_Linux.a

all : $(NAME)

$(NAME) : $(OBJS)
	@cc $(CFLAGS) $(OBJS) $(MLX_LIB) $(MLX_FLAGS) -o $@
	@printf "$(ERASE)$(GREEN)--> $@ made <--$(END)\n"

$(OBJS_DIR)/%.o: srcs/%.c Makefile includes/cub3d.h
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(BLUE) > Compilation :$(END) $<\r"

clean :
	@printf "$(MAGENTA)-->	$(OBJS_DIR) removed$(END)\n"
	@rm -rdf $(OBJS_DIR)

fclean : clean
	@printf "$(MAGENTA)-->	$(NAME) removed$(END)\n"
	@rm -f $(NAME)

re : fclean all
