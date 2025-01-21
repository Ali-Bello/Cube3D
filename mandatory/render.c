/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:38:39 by marvin            #+#    #+#             */
/*   Updated: 2025/01/21 21:01:40 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	get_texture_id(t_ray *ray)
{
	int	tex_id;

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

void	draw_bounds_line(t_game *game, t_point bounds, int x, int color)
{
	while (bounds.x < bounds.y)
	{
		ft_mlx_pixel_put(&game->render_buf, x, bounds.x, color);
		bounds.x++;
	}
}

float	set_texture_cordinates(t_ray *ray, t_img *texture, t_point *cords)
{
	float	step;

	if (ray->wall_hit_face)
		cords->x = (fmodf(ray->wall_hit.x, CUB_SIZE) / CUB_SIZE)
			* texture->width;
	else
		cords->x = (fmodf(ray->wall_hit.y, CUB_SIZE) / CUB_SIZE)
			* texture->width;
	cords->y = 0;
	step = (float)texture->height / (float)ray->casted.height;
	if (ray->casted.botm_y - ray->casted.top_y > WIN_HEIGHT)
	{
		cords->y = (float)(0 - ray->casted.top_y) * step;
		ray->casted.top_y = 0;
	}
	if (ray->casted.botm_y > WIN_HEIGHT)
		ray->casted.botm_y = WIN_HEIGHT;
	return (step);
}

int	shade_color(int color, float distance, bool is_vertical)
{
	int		r;
	int		g;
	int		b;
	float	shade_factor;

	shade_factor = fminf(1.0, 1.0 / (distance * 0.009));
	if (is_vertical)
		shade_factor *= 0.85;
	r = ((color >> 16) & 0xFF) * shade_factor;
	g = ((color >> 8) & 0xFF) * shade_factor;
	b = (color & 0xFF) * shade_factor;
	return ((r << 16) | (g << 8) | b);
}

void	draw_ray(t_game *game, t_ray *ray, int w_idx)
{
	t_point	tex_cord;
	t_img	*texture;
	int		i;
	float	step;

	i = -1;
	texture = &game->textures[get_texture_id(ray)];
	draw_bounds_line(game, (t_point){0, ray->casted.top_y, false}, w_idx,
		game->c_color);
	step = set_texture_cordinates(ray, texture, &tex_cord);
	i = ray->casted.top_y - 1.0;
	while (++i < ray->casted.botm_y)
	{
		ft_mlx_pixel_put(&game->render_buf, w_idx, i,
			shade_color(get_texture_pixel(texture, tex_cord.x, tex_cord.y),
				ray->casted.distance, ray->wall_hit_face));
		tex_cord.y += step;
	}
	draw_bounds_line(game, (t_point){ray->casted.botm_y, WIN_HEIGHT, false},
		w_idx, game->f_color);
}
