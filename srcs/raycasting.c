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

float   normalize_angle(float angle)
{
    angle = fmodf(angle, 2 * M_PI);
    if (angle < 0)
        angle += (2 * M_PI);
    return (angle);
}

float    distance_from_origin(t_player *player, float x, float y)
{
    return (sqrtf((x - player->x) * (x - player->x) + (y - player->y) * (y - player->y)));
}

t_point check_intersection(t_game *game, t_ray *ray, bool flag)
{
    t_point next_touch;

    next_touch = (t_point){0};
    while (ray->start.x >= 0 && ray->start.x < game->map_width * CUB_SIZE
        && ray->start.y >= 0 && ray->start.y < game->map_height * CUB_SIZE)
    {
        next_touch.x = ray->start.x;
        if (flag && ray->facing_left)
            next_touch.x -= 0.1;
        next_touch.y = ray->start.y;
        if (!flag && !ray->facing_down)
            next_touch.y -= 0.1;
        if (game->map[(int)(next_touch.y / CUB_SIZE)]\
        [(int)(next_touch.x / CUB_SIZE)] == '1')
            return ((t_point){ray->start.x, ray->start.y, true});
        else if (game->map[(int)(next_touch.y / CUB_SIZE)]\
        [(int)(next_touch.x / CUB_SIZE)] == 'D')
            return ((t_point){next_touch.x, next_touch.y, true});
        else
        {
            ray->start.x += ray->x_step;
            ray->start.y += ray->y_step;
        }
    }
    return (next_touch);
}

t_point set_vert_intercept(t_game *game, t_ray *ray)
{
    t_point intercept;

    intercept.is_hit = false;
    intercept.y = floorf(game->player.y / CUB_SIZE) * CUB_SIZE;
    if (ray->facing_down)
        intercept.y += CUB_SIZE;
    intercept.x = game->player.x + (intercept.y - game->player.y) / ray->angle_tan;
    ray->y_step = CUB_SIZE;
    if (ray->facing_down == false)
        ray->y_step *= -1;
    ray->x_step = CUB_SIZE / ray->angle_tan;
    if (ray->facing_left && ray->x_step > 0)
        ray->x_step *= -1;
    if (!ray->facing_left && ray->x_step < 0)
        ray->x_step *= -1;
    return (intercept);
}

t_point set_horiz_intercept(t_game *game, t_ray *ray)
{
    t_point intercept;

    intercept.is_hit = false;
    intercept.x = floorf(game->player.x / CUB_SIZE) * CUB_SIZE;
    if (!ray->facing_left)
        intercept.x += CUB_SIZE;
    intercept.y = game->player.y + (intercept.x - game->player.x) * ray->angle_tan;
    ray->x_step = CUB_SIZE;
    if (ray->facing_left)
        ray->x_step *= -1;
    ray->y_step = CUB_SIZE * ray->angle_tan;
    if (!ray->facing_down && ray->y_step > 0)
        ray->y_step *= -1;
    if (ray->facing_down && ray->y_step < 0)
        ray->y_step *= -1;
    return (intercept);
}

void    set_distances(t_game *game, t_ray *ray, float *horz, float *vert)
{
    if (ray->horiz_wall_hit.is_hit)
        *horz = distance_from_origin(&game->player, ray->horiz_wall_hit.x, ray->horiz_wall_hit.y);
    else
        *horz = INFINITY;
    if (ray->vert_wall_hit.is_hit)
        *vert = distance_from_origin(&game->player, ray->vert_wall_hit.x, ray->vert_wall_hit.y);
    else
        *vert = INFINITY;
}

void    perform_dda(t_game *game, t_ray *ray)
{
    float   horz_hit_distance;
    float   vert_hit_distance;

    set_distances(game, ray, &horz_hit_distance, &vert_hit_distance);
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

void    cast_ray(t_game *game, t_ray *ray, float angle)
{
    memset(ray, 0, sizeof(t_ray));

    angle = normalize_angle(angle);
    ray->angle_tan = tanf(angle);
    ray->facing_down = angle > 0 && angle < M_PI;
    ray->facing_left = angle > M_PI_2 && angle < 3 * M_PI_2;

    ray->start = set_vert_intercept(game, ray);
    ray->vert_wall_hit = check_intersection(game, ray, 0);

    ray->start = set_horiz_intercept(game, ray);
    ray->horiz_wall_hit = check_intersection(game, ray, 1);

    perform_dda(game, ray);
}
