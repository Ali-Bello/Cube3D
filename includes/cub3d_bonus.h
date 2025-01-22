/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:45:44 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/22 02:56:01 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include "cub3d.h"
# include "mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <limits.h>
# include <math.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef struct s_sprite
{
	t_img		img;
	float		angle;
	int			x;
	int			y;
	int			offset_x;
	int			frame_width;
	int			frame_height;
	int			delta;
	bool		is_visible;
	t_cast		casted;
}				t_sprite;

typedef struct s_world
{
	t_img		textures[5];
	t_sprite	*coins;
	int			n_coins;
	int			collected_coins;
}				t_world;

typedef struct s_bonus_game
{
	t_game		data;
	t_img		sky;
	t_sprite	portal;
	t_world		world[2];
	float		distances[WIN_WIDTH];
	int			backgroud_music_id;
	bool		spawn_portal;
	bool		portal_effect;
	bool		door_open;
	bool		door_inrange;
	bool		mouse_mode;
	int			world_idx;
}				t_bonus_game;

void			free_malloced_data(t_bonus_game *game);
void			update_player_position(t_bonus_game *game);
void			update_mouse_interaction(t_bonus_game *game);
float			get_casted_distance(t_bonus_game *game);
void			draw_rectangle(t_game *game, t_point pos, t_point dimens,
					int color);
void			draw_circle(t_game *game, t_point pos, int radius, int color);
//////////// SOUND /////////////

void			play_sound(t_bonus_game *game, char *path, char *gain);

///////////////////////////////////////

//////////// COLLECTIBLES /////////////

void			init_collectibles(t_bonus_game *game, int world_id, char *path);
void			update_collectibles(t_bonus_game *game);
void			generate_valid_coordinates(t_bonus_game *game, int *x, int *y);

///////////////////////////////////////

//////////// SPRITE ANIMATIONS /////////////

void			set_sprite_dimensions(t_bonus_game *game, t_sprite *sprite);
void			draw_sprite(t_bonus_game *game, t_sprite *sprite);

////////////////////////////////////////////

//////////// PORTAL /////////////

void			generate_valid_coordinates(t_bonus_game *game, int *x, int *y);
void			update_portal(t_bonus_game *game);

/////////////////////////////////

//////////// MECANICS /////////////

bool			wall_collision_check_bonus(t_bonus_game *game, float x,
					float y);

///////////////////////////////////

//////////// MINI_MAP /////////////

void			draw_mini_ray(t_bonus_game *game, t_ray *ray);
void			clear_mini_map_area(t_img *render_img);
void			draw_line(t_bonus_game *game, t_point start, t_point end,
					int color);
///////////////////////////////////////

//////////// INITIALIZING /////////////

void			init_bonus_game(t_bonus_game *data);
///////////////////////////////////////

////////////  RAYCASTING  /////////////

float			normalize_angle(float angle);
float			distance_from_origin(t_player *player, float x, float y);
void			cast_ray_bonus(t_bonus_game *game, t_ray *ray, float angle);
t_point			check_intersection_bonus(t_bonus_game *game, t_ray *ray,
					bool flag);
//////////////////////////////////////

////////////  EVENTS  /////////////////

int				exit_routine_bonus(t_bonus_game *game);
int				key_press_bonus(int key, t_bonus_game *game);
int				key_release_bonus(int key, t_bonus_game *game);
//////////////////////////////////////

////////////  RENDER  /////////////////

void			draw_mini_map(t_bonus_game *game);
void			draw_ray_bonus(t_bonus_game *game, t_ray *ray, int w_idx);
//////////////////////////////////////

#endif