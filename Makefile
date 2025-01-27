ERASE		:=  \033[2K\r
BLUE		:=	\033[34m
YELLOW		:=	\033[33m
GREEN		:=	\033[32m
MAGENTA		:=	\033[35m
CYAN		:=	\033[36m
BOLD		:=	\033[1m
END			:=	\033[0m

NAME = cub3D

BONUS_NAME = cub3d_bonus

CFLAGS = -Wall -Werror -Wextra -g3 #-fsanitize=address

MLX_FLAGS = -lXext -lX11 -lm

OBJS_DIR = objs

LIBFT_PATH = includes/libft

MLX_LIB = includes/libmlx_Linux.a

LIBFT = includes/libft/libft.a

PARSING_SRCS = srcs/parsing/parse.c srcs/parsing/parse_map.c srcs/parsing/map.c \
			srcs/parsing/map_walls.c srcs/parsing/textures.c srcs/parsing/colors.c \
			srcs/parsing/ft_error.c

MANDATORY_SRCS = srcs/mandatory/raycasting.c srcs/mandatory/pixels.c srcs/mandatory/events.c \
		srcs/mandatory/render.c srcs/mandatory/init.c srcs/mandatory/formulas.c \
		srcs/mandatory/main.c

BONUS_SRCS = srcs/bonus/events.c srcs/bonus/render.c srcs/bonus/init.c \
			srcs/bonus/collectibles.c srcs/bonus/mini_map.c srcs/bonus/portal.c \
			srcs/bonus/debugging.c srcs/bonus/raycasting.c srcs/bonus/movement.c \
			srcs/mandatory/formulas.c srcs/mandatory/render.c srcs/mandatory/init.c \
			srcs/mandatory/raycasting.c srcs/mandatory/events.c srcs/mandatory/pixels.c \
			srcs/bonus/main.c

COMBINED_SRCS = $(PARSING_SRCS) $(MANDATORY_SRCS)

COMBINED_BSRCS = $(PARSING_SRCS) $(BONUS_SRCS)

OBJS = $(patsubst srcs/%.c, $(OBJS_DIR)/%.o, $(COMBINED_SRCS))

BONUS_OBJS = $(patsubst srcs/%.c, $(OBJS_DIR)/%.o, $(COMBINED_BSRCS))


all : $(NAME)

$(NAME) : $(OBJS) $(LIBFT)
	@cc $(CFLAGS) $(OBJS) $(MLX_LIB) $(MLX_FLAGS) $(LIBFT) -o $@
	@printf "$(ERASE)$(GREEN)--> $@ made <--$(END)\n"

$(OBJS_DIR)/%.o: srcs/%.c Makefile includes/headers/cub3d.h includes/headers/parsing.h
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/mandatory $(OBJS_DIR)/parsing
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(BLUE) > Compilation :$(END) $<\r"

$(LIBFT): $(LIBFT_PATH)
	@make -C $(LIBFT_PATH) --no-print-directory

$(BONUS_NAME) : $(BONUS_OBJS) $(LIBFT_PATH)
	@cc $(CFLAGS) $(BONUS_OBJS) $(MLX_LIB) $(MLX_FLAGS) -o $@
	@printf "$(ERASE)$(GREEN)--> $@ made <--$(END)\n"

$(OBJS_DIR)/%.o: %.c Makefile includes/headers/cub3d.h includes/headers/parsing.h
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/mandatory $(OBJS_DIR)/parsing
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(BLUE) > Compilation :$(END) $<\r"

clean :
	@make -C $(LIBFT_PATH) clean --no-print-directory
	@printf "$(MAGENTA)-->	$(OBJS_DIR) removed$(END)\n"
	@rm -rdf $(OBJS_DIR)

fclean : clean
	@rm -f $(NAME) $(BONUS_NAME)
	@rm -f $(LIBFT)
	@printf "$(MAGENTA)-->	$(NAME) removed$(END)\n"

re : fclean all

bonus : $(BONUS_NAME)

.PHONY: all clean fclean re
.SECONDARY:	$(OBJS)