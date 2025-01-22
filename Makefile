ERASE		:=  \033[2K\r
BLUE		:=	\033[34m
YELLOW		:=	\033[33m
GREEN		:=	\033[32m
MAGENTA		:=	\033[35m
CYAN		:=	\033[36m
BOLD		:=	\033[1m
END			:=	\033[0m

NAME = cub3d

BONUS_NAME = cub3d_bonus

CFLAGS = -Wall -Werror -Wextra -g3 #-fsanitize=address

MLX_FLAGS = -lXext -lX11 -lm

SRCS = mandatory/raycasting.c mandatory/pixels.c mandatory/events.c mandatory/render.c\
		mandatory/init.c mandatory/formulas.c mandatory/main.c

BONUS_SRCS = bonus/events.c bonus/render.c bonus/init.c bonus/collectibles.c bonus/mini_map.c\
			bonus/portal.c bonus/debugging.c bonus/raycasting.c bonus/movement.c bonus/main.c mandatory/render.c\
			mandatory/init.c mandatory/raycasting.c mandatory/events.c mandatory/pixels.c mandatory/formulas.c

OBJS_DIR = objs

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

BONUS_OBJS = $(addprefix $(OBJS_DIR)/, $(BONUS_SRCS:.c=.o))

MLX_LIB = libmlx_Linux.a

all : $(NAME)

$(NAME) : $(OBJS)
	@cc $(CFLAGS) $(OBJS) $(MLX_LIB) $(MLX_FLAGS) -o $@
	@printf "$(ERASE)$(GREEN)--> $@ made <--$(END)\n"

$(OBJS_DIR)/mandatory/%.o: mandatory/%.c Makefile includes/cub3d.h
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/mandatory
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(BLUE) > Compilation :$(END) $<\r"

$(BONUS_NAME) : $(BONUS_OBJS)
	@cc $(CFLAGS) $(BONUS_OBJS) $(MLX_LIB) $(MLX_FLAGS) -o $@
	@printf "$(ERASE)$(GREEN)--> $@ made <--$(END)\n"

$(OBJS_DIR)/bonus/%.o: bonus/%.c Makefile includes/cub3d_bonus.h includes/cub3d.h
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/bonus
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(BLUE) > Compilation :$(END) $<\r"

clean :
	@printf "$(MAGENTA)-->	$(OBJS_DIR) removed$(END)\n"
	@rm -rdf $(OBJS_DIR)

fclean : clean
	@printf "$(MAGENTA)-->	$(NAME) removed$(END)\n"
	@rm -f $(NAME)
	@rm -f $(BONUS_NAME)

re : fclean all

bonus : $(BONUS_NAME)