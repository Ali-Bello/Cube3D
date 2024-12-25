/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:40:34 by marvin            #+#    #+#             */
/*   Updated: 2024/12/21 14:53:15 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

float  normalize_angle(float angle)
{
    angle = remainderf(angle, 2 * M_PI);
    if (angle < 0)
        angle += (2 * M_PI);
    return (angle);
}

float    distance_from_origin(t_player *player, float x, float y)
{
    return (sqrt((x - player->x) * (x - player->x) + (y - player->y) * (y - player->y)));
}

t_point check_intersect(t_game *game, t_ray *ray, bool flag)
{
    t_point next_touch;

    next_touch = (t_point){0};
    while (ray->start.x >= 0 && ray->start.x < game->win_width
        && ray->start.y >= 0 && ray->start.y < game->win_height)
    {
        next_touch.x = ray->start.x;
        if (flag && ray->facing_left)
            next_touch.x--;
        next_touch.y = ray->start.y;
        if (!flag && !ray->facing_down)
            next_touch.y--;
        if (game->map[(int)floorf(next_touch.y / TILE_SIZE)]\
        [(int)floorf(next_touch.x / TILE_SIZE)] == '1')
            return ((void)(next_touch.is_hit = true), next_touch);
        else
        {
            ray->start.x += ray->x_step;
            ray->start.y += ray->y_step;
        }
    }
    return (next_touch);
}

t_point set_horiz_intercept(t_game *game, t_ray *ray)
{
    t_point intercept;

    intercept.is_hit = false;
    intercept.y = floorf(game->player.y / TILE_SIZE) * TILE_SIZE;
    if (ray->facing_down)
        intercept.y += TILE_SIZE;
    intercept.x = game->player.x + (intercept.y - game->player.y) / ray->tan_ray_angle;
    ray->y_step = TILE_SIZE;
    if (ray->facing_down == false)
        ray->y_step *= -1;
    ray->x_step = TILE_SIZE / ray->tan_ray_angle;
    if (ray->facing_left && ray->x_step > 0)
        ray->x_step *= -1;
    if (!ray->facing_left && ray->x_step < 0)
        ray->x_step *= -1;
    return (intercept);
}

t_point set_vert_intercept(t_game *game, t_ray *ray)
{
    t_point intercept;

    intercept.is_hit = false;
    intercept.x = floorf(game->player.x / TILE_SIZE) * TILE_SIZE;
    if (!ray->facing_left)
        intercept.x += TILE_SIZE;
    intercept.y = game->player.y + (intercept.x - game->player.x) * ray->tan_ray_angle;
    ray->x_step = TILE_SIZE;
    if (ray->facing_left)
        ray->x_step *= -1;
    ray->y_step = TILE_SIZE * ray->tan_ray_angle;
    if (!ray->facing_down && ray->y_step > 0)
        ray->y_step *= -1;
    if (ray->facing_down && ray->y_step < 0)
        ray->y_step *= -1;
    return (intercept);
}

void    perform_dda(t_game *game, t_ray *ray)
{
    float   horz_hit_distance;
    float   vert_hit_distance;

    if (ray->horiz_wall_hit.is_hit)
        horz_hit_distance = distance_from_origin(&game->player, ray->horiz_wall_hit.x, ray->horiz_wall_hit.y);
    else
        horz_hit_distance = (float)INT_MAX;
    if (ray->vert_wall_hit.is_hit)
        vert_hit_distance = distance_from_origin(&game->player, ray->vert_wall_hit.x, ray->vert_wall_hit.y);
    else
        vert_hit_distance = (float)INT_MAX;
    if (horz_hit_distance < vert_hit_distance)
    {
        ray->distance = horz_hit_distance;
        ray->wall_hit.x = ray->horiz_wall_hit.x;
        ray->wall_hit.y = ray->horiz_wall_hit.y;
    }
    else
    {
        ray->distance = vert_hit_distance;
        ray->wall_hit.x = ray->vert_wall_hit.x;
        ray->wall_hit.y = ray->vert_wall_hit.y;
        ray->wall_hit_face = true;
    }
}

void    cast_ray(t_game *game, t_ray *ray, float ray_angle)
{
    memset(ray, 0, sizeof(t_ray));

    ray_angle = normalize_angle(ray_angle);
    ray->tan_ray_angle = tanf(ray_angle);
    ray->facing_down = ray_angle > 0 && ray_angle < M_PI;
    ray->facing_left = ray_angle > M_PI_2 && ray_angle < 3 * M_PI_2;

    ray->start = set_horiz_intercept(game, ray);
    ray->horiz_wall_hit = check_intersect(game, ray, 0);

    ray->start = set_vert_intercept(game, ray);
    ray->vert_wall_hit = check_intersect(game, ray, 1);

    perform_dda(game, ray);
}
