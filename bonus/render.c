/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:38:39 by marvin            #+#    #+#             */
/*   Updated: 2025/01/21 02:40:44 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	get_texture_id_bonus(t_bonus_game *game, t_ray *ray)
{
	int	tex_id;

	if (game->data.map[(int)(ray->wall_hit.y / CUB_SIZE)][(int)(ray->wall_hit.x
			/ CUB_SIZE)] == 'D')
		return (4);
	if (!ray->wall_hit_face)
	{
		if (!ray->facing_left)
			tex_id = 0;
		else
			tex_id = 1;
	}
	else
	{
		if (!ray->facing_down)
			tex_id = 2;
		else
			tex_id = 3;
	}
	return (tex_id);
}

void	draw_floor(t_bonus_game *game, t_point bounds, int x, int color)
{
	while (bounds.x < bounds.y)
	{
		if (bounds.x >= MINI_MAP_SIZE || x >= MINI_MAP_SIZE)
			ft_mlx_pixel_put(&game->data.render_buf, x, bounds.x, color);
		bounds.x++;
	}
}

void	draw_ceiling(t_bonus_game *game, t_ray *ray, int w_idx)
{
	float	sky_offset;
	int		i;

	i = -1;
	sky_offset = (game->data.player.angle.rad / (2 * M_PI)) * game->sky.width;
	while (++i < ray->casted.top_y)
	{
		ft_mlx_pixel_put(&game->data.render_buf, w_idx, i,
			get_texture_pixel(&game->sky, (w_idx + (int)sky_offset)
				% game->sky.width, (i * game->sky.height) / (WIN_HEIGHT / 2)));
	}
}

void	draw_ray_bonus(t_bonus_game *game, t_ray *ray, int w_idx)
{
	t_point	tex_cord;
	t_img	*texture;
	int		i;
	float	step;
	int		color;

	draw_ceiling(game, ray, w_idx);
	texture = &game->world[game->world_idx].textures[get_texture_id_bonus(game,
			ray)];
	step = set_texture_cordinates(ray, texture, &tex_cord);
	i = ray->casted.top_y - 1.0;
	while (++i < ray->casted.botm_y)
	{
		color = get_texture_pixel(texture, tex_cord.x, tex_cord.y);
		if (color >= 0)
			ft_mlx_pixel_put(&game->data.render_buf, w_idx, i,
				shade_color(color, ray->casted.distance, ray->wall_hit_face));
		tex_cord.y += step;
	}
	if (game->portal_effect)
		draw_floor(game, (t_point){ray->casted.botm_y,
			WIN_HEIGHT, true}, w_idx, 0x4D3823);
	else
		draw_floor(game, (t_point){ray->casted.botm_y,
			WIN_HEIGHT, true}, w_idx, 0x8C6756);
}
