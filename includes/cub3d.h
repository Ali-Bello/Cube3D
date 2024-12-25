/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 23:40:00 by marvin            #+#    #+#             */
/*   Updated: 2024/12/21 14:54:43 by aderraj          ###   ########.fr       */
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
#define SCALE_FACTOR 0.25

typedef struct s_img
{
    void    *img;
    char    *addr;
    int     width;
    int     height;
    int     bpp;
    int     line_len;
    int     endian;
} t_img;

typedef struct s_player
{
    float   x;
    float   y;
    float   radius;
    float   mov_speed;
    float   rot_speed;
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
    float   tan_ray_angle;
    float   x_step;
    float   y_step;
    float   distance;
    float   plane_distance;
    int     wall_height;
    int     top_px;
    int     botm_px;
    bool    facing_down;
    bool    facing_left;
    bool    wall_hit_face;
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
    t_img   textures;
    t_ray   *rays;
} t_game;

    //////////// INITIALIZING /////////////

void    init_game(t_game *data);
void    check_allocations(t_game *data);

    ///////////////////////////////////////

    ////////////  RAYCASTING  /////////////

float   normalize_angle(float angle);
float   distance_from_origin(t_player *player, float x, float y);
void    cast_ray(t_game *game, t_ray *ray, float ray_angle);

    //////////////////////////////////////

    ////////////  DEBUGGING  //////////////

void    draw_square(t_game *game, int x, int y, int size, int color);
void    draw_circle(t_game *game, int x, int y, int radius, int color);
void    draw_line(t_game *game, int x0, int y0, int x1, int y1, int color);

    //////////////////////////////////////

    ////////////  EVENTS  /////////////////

int exit_routine(t_game *game);
int key_press(int key, t_game *game);
int key_release(int key, t_game *game);

    //////////////////////////////////////

    ////////////  RENDER  /////////////////

void    ft_mlx_pixel_put(t_img *data, int x, int y, int color);
void	ft_cpy_pixel(t_img *src, t_img *dst, int x, int y);
void    draw_map(t_game *game);
void    draw_player(t_game *game);
void    draw_ray(t_game *game, t_ray *ray, int w_idx);
void    draw_rectangle(t_game *game, int x, int y, int width, int height, int color);
void    clear_buf_img(t_game *game);
    //////////////////////////////////////

#endif