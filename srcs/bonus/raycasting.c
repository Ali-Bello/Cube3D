/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:40:34 by marvin            #+#    #+#             */
/*   Updated: 2025/01/28 23:28:23 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d_bonus.h"

t_point	check_intersection_bonus(t_bonus_game *game, t_ray *ray, bool flag)
{
	t_point	next_touch;
	size_t	width;

	next_touch = (t_point){0};
	width = get_map_line_width(&game->data, ray);
	while (ray->start.x >= 0 && ray->start.x < width * CUB_SIZE
		&& ray->start.y >= 0 && ray->start.y < game->data.map_height * CUB_SIZE)
	{
		next_touch.x = ray->start.x;
		if (flag && ray->facing_left)
			next_touch.x -= 0.0001f;
		next_touch.y = ray->start.y;
		if (!flag && !ray->facing_down)
			next_touch.y -= 0.0001f;
		if (game->data.map[(int)(next_touch.y / CUB_SIZE)][(int)(next_touch.x
				/ CUB_SIZE)] == '1' || game->data.map[(int)(next_touch.y
				/ CUB_SIZE)][(int)(next_touch.x / CUB_SIZE)] == 'D')
			return ((t_point){next_touch.x, next_touch.y, true});
		ray->start.x += ray->x_step;
		ray->start.y += ray->y_step;
		width = get_map_line_width(&game->data, ray);
	}
	return (next_touch);
}

void	cast_ray_bonus(t_bonus_game *game, t_ray *ray, float angle)
{
	ft_memset(ray, 0, sizeof(t_ray));
	angle = normalize_angle(angle);
	ray->angle_tan = tanf(angle);
	ray->facing_down = angle > 0 && angle < M_PI;
	ray->facing_left = angle > M_PI_2 && angle < 3 * M_PI_2;
	ray->start = set_vert_intercept(&game->data, ray);
	ray->vert_wall_hit = check_intersection_bonus(game, ray, 0);
	ray->start = set_horiz_intercept(&game->data, ray);
	ray->horiz_wall_hit = check_intersection_bonus(game, ray, 1);
	perform_dda(&game->data, ray);
	ray->casted.height = (CUB_SIZE / (ray->casted.distance * cosf(angle
					- game->data.player.angle.rad))) * game->data.perp_distance;
	ray->casted.top_y = (WIN_HEIGHT - ray->casted.height) / 2.0;
	ray->casted.botm_y = ray->casted.top_y + ray->casted.height;
}
