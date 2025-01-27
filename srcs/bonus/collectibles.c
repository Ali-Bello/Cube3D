/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collectibles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:55:28 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/27 21:28:51 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d_bonus.h"

int	compare_collectibles(const void *a, const void *b)
{
	t_sprite	*sprite_a;
	t_sprite	*sprite_b;

	sprite_a = (t_sprite *)a;
	sprite_b = (t_sprite *)b;
	if (sprite_a->casted.distance < sprite_b->casted.distance)
		return (1);
	else if (sprite_a->casted.distance > sprite_b->casted.distance)
		return (-1);
	else
		return (0);
}

void	set_sprites_distances(t_bonus_game *game)
{
	int	i;

	i = -1;
	while (++i < game->world[game->world_idx].n_coins)
	{
		game->world[game->world_idx].coins[i].casted.distance
			= distance_from_origin(&game->data.player,
				game->world[game->world_idx].coins[i].x,
				game->world[game->world_idx].coins[i].y)
			* cosf(atan2f(game->world[game->world_idx].coins[i].y
					- game->data.player.y,
					game->world[game->world_idx].coins[i].x
					- game->data.player.x) - game->data.player.angle.rad);
	}
	qsort(game->world[game->world_idx].coins,
		game->world[game->world_idx].n_coins, sizeof(t_sprite),
		compare_collectibles);
}

void	render_collectibles(t_bonus_game *game, int i)
{
	if (game->world[game->world_idx].coins[i].offset_x
		>= game->world[game->world_idx].coins[i].img.width)
		game->world[game->world_idx].coins[i].offset_x = 0;
	set_sprite_dimensions(game, &game->world[game->world_idx].coins[i]);
	draw_sprite(game, &game->world[game->world_idx].coins[i]);
	if (game->world[game->world_idx].coins[i].delta % 15 == 0)
		game->world[game->world_idx].coins[i].offset_x
			+= game->world[game->world_idx].coins[i].frame_width;
	game->world[game->world_idx].coins[i].delta++;
}

void	update_collectibles(t_bonus_game *game)
{
	int	i;

	i = 0;
	set_sprites_distances(game);
	while (i < game->world[game->world_idx].n_coins)
	{
		if (game->world[game->world_idx].coins[i].is_visible
			&& game->world[game->world_idx].coins[i].casted.distance <= 10.0)
		{
			game->world[game->world_idx].collected_coins++;
			game->world[game->world_idx].coins[i].is_visible = false;
			game->world[game->world_idx].coins[i].x = -1;
			game->world[game->world_idx].coins[i].y = -1;
			play_sound(game, "./assets/sfx/coin.mp3", "0.15");
		}
		else if (game->world[game->world_idx].coins[i].is_visible)
			render_collectibles(game, i);
		i++;
	}
}
