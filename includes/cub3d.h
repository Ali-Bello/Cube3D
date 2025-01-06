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
#define CUB_SIZE 64 
#define FOV 60 * (M_PI / 180)
#define WIN_WIDTH  1280
#define WIN_HEIGHT 720
#define WALK_SPEED 0.5
#define ROT_SPEED 0.5 * (M_PI / 180)
#define MINI_MAP_SIZE 150
#define MINI_MAP_SCALE_FACTOR 0.40

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define BLUE "\x1b[34m"
# define YELLOW "\x1b[33m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define RESET "\x1b[0m"
# define BOLD "\x1b[1m"
# define ITALIC "\x1b[3m"
# define UNDERLINE "\x1b[4m"

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
    float   rot_angle;
    char    walk_dir; // 1 walk forward, -1 walk backward 
    char    turn_dir; // 1 turn right, -1 turn left
    int     mouse_x;
    int     mouse_y;
} t_player;

typedef struct s_point
{
    float   x;
    float   y;
    bool    is_hit;
} t_point;

typedef struct s_ray
{
    float   angle_tan;
    float   x_step;
    float   y_step;
    float   distance;
    float   plane_distance;
    float     wall_height;
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
    void        *mlx;
    void        *win;
    int         map_width;
    int         map_height;
    char        **map;
    t_player    player;
    t_img       render_buf;
    t_img       textures[5];
} t_game;

    //////////// INITIALIZING /////////////

void    init_game(t_game *data);
void    check_allocations(t_game *data);

    ///////////////////////////////////////

    ////////////  RAYCASTING  /////////////

float   normalize_angle(float angle);
float   distance_from_origin(t_player *player, float x, float y);
void    cast_ray(t_game *game, t_ray *ray, float angle);

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
int mouse_move(int x, int y, t_game *game);
    //////////////////////////////////////

    ////////////  RENDER  /////////////////

void    ft_mlx_pixel_put(t_img *data, int x, int y, int color);
void    draw_mini_map(t_game *game);
void    draw_player(t_game *game);
void    draw_ray(t_game *game, t_ray *ray, int w_idx);
void    draw_rectangle(t_game *game, int x, int y, int width, int height, int color);
void    clear_buf_img(t_game *game);
    //////////////////////////////////////

#endif