/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 23:40:00 by marvin            #+#    #+#             */
/*   Updated: 2024/12/11 23:40:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include "mlx.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define D 100
#define W 119
#define A 97
#define S 115
#define LEFT 65361
#define RIGHT 65363
#define ESC 65307
#define TILE_SIZE 64
#define FOV 60 * (M_PI / 180)
#define MINIMAP_SCALE_FACTOR 0.5

typedef struct s_img
{
    void    *img;
    char    *addr;
    int     bpp;
    int     line_len;
    int     endian;
} t_img;

typedef struct s_player
{
    double   x;
    double   y;
    double   radius;
    double   mov_speed;
    double   rot_speed;
    float   rot_angle;
    char    walk_dir; // 1 walk forward, -1 walk backward 
    char    turn_dir; // 1 turn right, -1 turn left
} t_player;

typedef struct s_point
{
    float   x;
    float   y;
    bool    is_hit;
} t_point;

typedef struct s_ray
{
    bool    facing_left;
    bool    facing_down;
    float   ray_angle;
    float   tan_ray_angle;
    float   x_step;
    float   y_step;
    float   distance;
    t_point horiz_wall_hit;
    t_point vert_wall_hit;
    t_point wall_hit;
    t_point start;
} t_ray;

typedef struct s_game
{
    void    *mlx;
    void    *win;
    int     win_width;
    int     win_height;
    char    **map;
    t_player player;
    t_img   render_buf;
} t_game;


    ////////////  RAYCASTING  /////////////

float  normalize_angle(float angle);
float    distance_from_origin(t_player *player, float x, float y);
void    cast_ray(t_game *game, t_ray *ray, float ray_angle);
    //////////////////////////////////////

    ////////////  DEBUGGING  //////////////

void    draw_square(t_game *game, int x, int y, int size, int color);
void    draw_circle(t_game *game, int x, int y, int radius, int color);
void    draw_line(t_game *game, int x, int y, int color);

    //////////////////////////////////////

    ////////////  EVENTS  /////////////////

int exit_routine(t_game *game);
int key_press(int key, t_game *game);
int key_release(int key, t_game *game);

    //////////////////////////////////////

    ////////////  RENDER  /////////////////

void    ft_mlx_pixel_put(t_img *data, int x, int y, int color);
void    draw_map(t_game *game);
void    draw_player(t_game *game);
void    draw_rectangle(t_game *game, int x, int y, int width, int height, int color);
void    clear_buf_img(t_game *game);
    //////////////////////////////////////

#endif