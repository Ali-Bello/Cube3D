/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 02:13:09 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/27 18:54:48 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d_bonus.h"

void	draw_rectangle(t_game *game, t_point pos, t_point dimens, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < dimens.y)
	{
		j = 0;
		while (j < dimens.x)
		{
			if (pos.x + j >= 0 && pos.x + j <= MINI_MAP_SIZE && pos.y + i >= 0
				&& pos.y + i <= MINI_MAP_SIZE)
				ft_mlx_pixel_put(&game->render_buf, pos.x + j, pos.y + i,
					color);
			j++;
		}
		i++;
	}
}

void	draw_circle(t_game *game, t_point pos, int radius, int color)
{
	int	i;
	int	j;

	i = -radius;
	while (i < radius)
	{
		j = -radius;
		while (j < radius)
		{
			if ((i * i) + (j * j) < radius * radius)
				ft_mlx_pixel_put(&game->render_buf, pos.x + j, pos.y + i,
					color);
			j++;
		}
		i++;
	}
}

void	init_line_pos(t_point *d, t_point *s, t_point start, t_point end)
{
	d->x = fabsf(end.x - start.x);
	d->y = fabsf(end.y - start.y);
	s->x = (start.x < end.x) * 1 + (start.x >= end.x) * -1;
	s->y = (start.y < end.y) * 1 + (start.y >= end.y) * -1;
}

void	draw_line(t_bonus_game *game, t_point start, t_point end, int color)
{
	t_point	d;
	t_point	s;
	int		err;
	int		e2;

	init_line_pos(&d, &s, start, end);
	err = d.x - d.y;
	while (1)
	{
		ft_mlx_pixel_put(&game->data.render_buf, start.x, start.y, color);
		if ((start.x == end.x && start.y == end.y) || (start.x >= MINI_MAP_SIZE
				- 1 || start.y >= MINI_MAP_SIZE - 1))
			break ;
		e2 = err * 2;
		if (e2 > -d.y)
		{
			err -= d.y;
			start.x += s.x;
		}
		if (e2 < d.x)
		{
			err += d.x;
			start.y += s.y;
		}
	}
}

float	get_casted_distance(t_bonus_game *game)
{
	return (distance_from_origin(&game->data.player, game->portal.x,
			game->portal.y) * cosf(atan2f(game->portal.y - game->data.player.y,
				game->portal.x - game->data.player.x)
			- game->data.player.angle.rad));
}
