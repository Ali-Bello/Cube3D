/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 23:40:00 by marvin            #+#    #+#             */
/*   Updated: 2025/01/27 21:29:42 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "colors.h"
# include "parsing.h"
# include "mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

# define CUB_SIZE 32
# define FOV 60
# define WIN_WIDTH 1280
# define WIN_HEIGHT 720
# define WALK_SPEED 1.0
# define ROT_SPEED WALK_SPEED
# define MINI_MAP_SIZE 150
# define MINI_MAP_SCALE_FACTOR 0.6

typedef struct s_point
{
	float		x;
	float		y;
	bool		is_hit;
}				t_point;

typedef struct s_img
{
	void		*img;
	char		*addr;
	int			width;
	int			height;
	int			bpp;
	int			line_len;
	int			endian;
}				t_img;

typedef struct s_angle
{
	float		rad;
	float		cos;
	float		sin;
	float		tan;
}				t_angle;

typedef struct s_cast
{
	float		distance;
	float		height;
	float		top_y;
	float		botm_y;
	float		left_x;
	float		right_x;
}				t_cast;

typedef struct s_ray
{
	float		angle_tan;
	float		x_step;
	float		y_step;
	bool		facing_down;
	bool		facing_left;
	bool		wall_hit_face;
	t_cast		casted;
	t_point		horiz_wall_hit;
	t_point		vert_wall_hit;
	t_point		wall_hit;
	t_point		start;
}				t_ray;

typedef struct s_player
{
	float		x;
	float		y;
	t_angle		angle;
	char		walk_dir;
	char		strafe_dir;
	char		turn_dir;
}				t_player;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	int			map_height;
	char		**map;
	t_player	player;
	t_img		render_buf;
	t_img		textures[4];
	int			f_color;
	int			c_color;
	float		player_spawn_dir;
	float		perp_distance;
}				t_game;

/*
 ********* INITIALIZING
*/

int				init_game(t_game *data, t_parse *parse);
int				load_image(t_game *data, t_img *img, char *path);
void			get_parsed_data(t_game *game, t_parse *parse);

/*
 ********** RAYCASTING 
*/

float			normalize_angle(float angle);
float			distance_from_origin(t_player *player, float x, float y);
void			cast_ray(t_game *game, t_ray *ray, float angle);
t_point			set_horiz_intercept(t_game *game, t_ray *ray);
t_point			set_vert_intercept(t_game *game, t_ray *ray);
t_point			check_intersection(t_game *game, t_ray *ray, bool flag);
void			perform_dda(t_game *game, t_ray *ray);
size_t			get_map_line_width(t_game *game, t_ray *ray);
void			set_distances(t_game *game, t_ray *ray, float *horz,
					float *vert);

/*
 ********** RENDERING
*/

void			ft_mlx_pixel_put(t_img *data, int x, int y, int color);
void			draw_ray(t_game *game, t_ray *ray, int w_idx);
int				get_texture_pixel(t_img *texture, int x, int y);
int				shade_color(int color, float distance, bool is_vertical);
float			set_texture_cordinates(t_ray *ray, t_img *texture,
					t_point *cords);

/*
 ********** GAME MECHANISM 
*/

bool			wall_collision_check(t_game *game, float x, float y);
bool			check_surrondings(char **map, float x, float y, char c);

/*
 ********** EVENTS
*/

int				exit_routine(t_game *game);
int				key_press(int key, t_game *game);
int				key_release(int key, t_game *game);

#endif