/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 02:55:33 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/27 18:55:33 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d_bonus.h"

void	clear_mini_map_area(t_img *render_img)
{
	int	i;
	int	j;

	i = -1;
	while (++i < MINI_MAP_SIZE)
	{
		j = -1;
		while (++j < MINI_MAP_SIZE)
			ft_mlx_pixel_put(render_img, j, i, 0x000000);
	}
}

void	draw_mini_ray(t_bonus_game *game, t_ray *ray)
{
	int	mini_x;
	int	mini_y;

	mini_x = ray->wall_hit.x * MINI_MAP_SCALE_FACTOR + (int)(MINI_MAP_SIZE / 2
			- (floorf(game->data.player.x) * MINI_MAP_SCALE_FACTOR));
	mini_y = ray->wall_hit.y * MINI_MAP_SCALE_FACTOR + (int)(MINI_MAP_SIZE / 2
			- (floorf(game->data.player.y) * MINI_MAP_SCALE_FACTOR));
	draw_line(game, (t_point){MINI_MAP_SIZE / 2, MINI_MAP_SIZE / 2, true},
		(t_point){mini_x, mini_y, true}, 0x3a3a50);
}

void	draw_mini_sprite(t_bonus_game *game, t_sprite *sprite, t_point offset)
{
	int	color;

	if (sprite->x < 0 || sprite->y < 0 || (sprite == &game->portal
			&& !game->spawn_portal))
		return ;
	else if (sprite == &game->portal)
		color = 0x00FF00;
	else
		color = 0xFFD700;
	sprite->angle = normalize_angle(game->data.player.angle.rad
			- atan2f(sprite->y - game->data.player.y, sprite->x
				- game->data.player.x));
	sprite->angle = fabsf(sprite->angle);
	if (sprite->angle < (FOV * M_PI / 180) / 2 || sprite->angle > (2 * M_PI
			- (FOV * M_PI / 180) / 2))
		sprite->is_visible = true;
	else
		sprite->is_visible = false;
	draw_rectangle(&game->data, (t_point){sprite->x * MINI_MAP_SCALE_FACTOR
		+ offset.x, sprite->y * MINI_MAP_SCALE_FACTOR + offset.y, true},
		(t_point){5, 5, true}, (color * sprite->is_visible)
		+ (!sprite->is_visible * 0x807e7a));
}

void	draw_map_content(t_bonus_game *game, t_point pos, t_point offset,
		int size)
{
	if (game->data.map[(int)pos.y][(int)pos.x] == '1')
		draw_rectangle(&game->data, (t_point){(int)pos.x * CUB_SIZE
			* MINI_MAP_SCALE_FACTOR + offset.x, (int)pos.y * CUB_SIZE
			* MINI_MAP_SCALE_FACTOR + offset.y, true}, (t_point){size, size,
			true}, 0xAAAAAAA);
	else if (game->data.map[(int)pos.y][(int)pos.x] == 'D')
		draw_rectangle(&game->data, (t_point){pos.x * CUB_SIZE
			* MINI_MAP_SCALE_FACTOR + offset.x, pos.y * CUB_SIZE
			* MINI_MAP_SCALE_FACTOR + offset.y, true}, (t_point){size - 1, size
			/ 2, true}, 0x4f6b82);
}

void	draw_mini_map(t_bonus_game *game)
{
	t_point	offset;
	int		scaled_size;
	int		i;
	int		j;

	offset.x = MINI_MAP_SIZE / 2 - (game->data.player.x
			* MINI_MAP_SCALE_FACTOR);
	offset.y = MINI_MAP_SIZE / 2 - (game->data.player.y
			* MINI_MAP_SCALE_FACTOR);
	scaled_size = CUB_SIZE * MINI_MAP_SCALE_FACTOR + 1;
	i = -1;
	while (game->data.map[++i])
	{
		j = -1;
		while (game->data.map[i][++j])
			draw_map_content(game, (t_point){j, i, true}, offset, scaled_size);
	}
	draw_mini_sprite(game, &game->portal, offset);
	i = -1;
	while (++i < game->world[game->world_idx].n_coins)
		draw_mini_sprite(game, &game->world[game->world_idx].coins[i], offset);
	draw_circle(&game->data, (t_point){MINI_MAP_SIZE / 2, MINI_MAP_SIZE / 2,
		true}, 7.5 * MINI_MAP_SCALE_FACTOR, 0xFF0000);
}
