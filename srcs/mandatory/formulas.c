/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formulas.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 21:56:41 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/28 00:38:11 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

float	normalize_angle(float angle)
{
	angle = fmodf(angle, 2 * M_PI);
	if (angle < 0)
		angle += (2 * M_PI);
	return (angle);
}

float	distance_from_origin(t_player *player, float x, float y)
{
	return (sqrtf(((x - player->x) * (x - player->x)) + ((y - player->y) * (y
					- player->y))));
}

size_t	get_map_line_width(t_game *game, t_ray *ray)
{
	size_t	width;

	if (ray->start.y >= 0 && ray->start.y / CUB_SIZE < game->map_height
		&& game->map[(int)(ray->start.y / CUB_SIZE)])
		width = ft_strlen(game->map[(int)(ray->start.y / CUB_SIZE)]);
	else
		width = 0;
	return (width);
}

void	set_distances(t_game *game, t_ray *ray, float *horz, float *vert)
{
	if (ray->horiz_wall_hit.is_hit)
		*horz = distance_from_origin(&game->player, ray->horiz_wall_hit.x,
				ray->horiz_wall_hit.y);
	else
		*horz = (float)INT_MAX;
	if (ray->vert_wall_hit.is_hit)
		*vert = distance_from_origin(&game->player, ray->vert_wall_hit.x,
				ray->vert_wall_hit.y);
	else
		*vert = (float)INT_MAX;
}
