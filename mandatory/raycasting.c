/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:40:34 by marvin            #+#    #+#             */
/*   Updated: 2025/01/20 21:57:43 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_point	check_intersection(t_game *game, t_ray *ray, bool flag)
{
	t_point	next_touch;
	size_t	width;

	next_touch = (t_point){0};
	width = get_map_width(game, ray);
	while (ray->start.x >= 0 && ray->start.x < width * CUB_SIZE
		&& ray->start.y >= 0 && ray->start.y < game->map_height * CUB_SIZE)
	{
		next_touch.x = ray->start.x;
		if (flag && ray->facing_left)
			next_touch.x -= 0.1;
		next_touch.y = ray->start.y;
		if (!flag && !ray->facing_down)
			next_touch.y -= 0.1;
		if (game->map[(int)(next_touch.y / CUB_SIZE)][(int)(next_touch.x
				/ CUB_SIZE)] == '1')
			return ((t_point){next_touch.x, next_touch.y, true});
		ray->start.x += ray->x_step;
		ray->start.y += ray->y_step;
		width = get_map_width(game, ray);
	}
	return (next_touch);
}

t_point	set_vert_intercept(t_game *game, t_ray *ray)
{
	t_point	intercept;

	intercept.is_hit = false;
	intercept.y = floorf(game->player.y / CUB_SIZE) * CUB_SIZE;
	if (ray->facing_down)
		intercept.y += CUB_SIZE;
	intercept.x = game->player.x + (intercept.y - game->player.y)
		/ ray->angle_tan;
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

t_point	set_horiz_intercept(t_game *game, t_ray *ray)
{
	t_point	intercept;

	intercept.is_hit = false;
	intercept.x = floorf(game->player.x / CUB_SIZE) * CUB_SIZE;
	if (!ray->facing_left)
		intercept.x += CUB_SIZE;
	intercept.y = game->player.y + (intercept.x - game->player.x)
		* ray->angle_tan;
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

void	perform_dda(t_game *game, t_ray *ray)
{
	float	horz_hit_distance;
	float	vert_hit_distance;

	set_distances(game, ray, &horz_hit_distance, &vert_hit_distance);
	if (horz_hit_distance < vert_hit_distance)
	{
		ray->casted.distance = horz_hit_distance;
		ray->wall_hit.x = ray->horiz_wall_hit.x;
		ray->wall_hit.y = ray->horiz_wall_hit.y;
	}
	else
	{
		ray->casted.distance = vert_hit_distance;
		ray->wall_hit.x = ray->vert_wall_hit.x;
		ray->wall_hit.y = ray->vert_wall_hit.y;
		ray->wall_hit_face = true;
	}
}

void	cast_ray(t_game *game, t_ray *ray, float angle)
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
	ray->casted.height = (CUB_SIZE / (ray->casted.distance * cosf(angle
					- game->player.angle.rad))) * game->perp_distance;
	ray->casted.top_y = (WIN_HEIGHT - ray->casted.height) / 2.0;
	ray->casted.botm_y = ray->casted.top_y + ray->casted.height;
}
