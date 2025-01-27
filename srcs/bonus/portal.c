/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 01:48:42 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/27 20:18:15 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d_bonus.h"

void    generate_valid_coordinates(t_bonus_game *game, int *x, int *y)
{
    size_t  width;
    int     map_x;
    int     map_y;
    bool    valid;

    valid = false;
    while (!valid)
    {
        map_y = rand() % game->data.map_height;
        width = ft_strlen(game->data.map[map_y]);
        map_x = rand() % width;
        *x = map_x * CUB_SIZE + (CUB_SIZE / 2);
        *y = map_y * CUB_SIZE + (CUB_SIZE / 2);
        if (game->data.map[map_y][map_x] == '0' && 
            !wall_collision_check_bonus(game, *x, *y))
            valid = true;
    }
}

void	teleport_player(t_bonus_game *game)
{
	int	new_x;
	int	new_y;

	if (game->portal.casted.distance <= 10.0 && game->portal.is_visible)
	{
		game->spawn_portal = false;
		generate_valid_coordinates(game, &new_x, &new_y);
		game->portal_effect = !game->portal_effect;
		game->data.player.x = new_x;
		game->data.player.y = new_y;
		system("pkill vlc");
		play_sound(game, "./assets/sfx/portal_sfx.mp3", "0.2");
		game->world_idx = !game->world_idx;
		if (game->world_idx)
			play_sound(game, "./assets/sfx/background1.mp3", "0.09");
		else
			play_sound(game, "./assets/sfx/background.mp3", "0.09");
		game->portal.x = 0;
		game->portal.y = 0;
	}
}

void	set_sprite_dimensions(t_bonus_game *game, t_sprite *sprite)
{
	sprite->casted.height = (CUB_SIZE / sprite->casted.distance)
		* game->data.perp_distance;
	sprite->casted.top_y = (WIN_HEIGHT - sprite->casted.height) / 2.0;
	sprite->casted.botm_y = sprite->casted.top_y + sprite->casted.height;
	if (sprite->casted.botm_y - sprite->casted.top_y > WIN_HEIGHT)
		sprite->casted.top_y = 0;
	if (sprite->casted.botm_y > WIN_HEIGHT)
		sprite->casted.botm_y = WIN_HEIGHT;
	sprite->casted.left_x = tanf(atan2f(sprite->y - game->data.player.y,
				sprite->x - game->data.player.x) - game->data.player.angle.rad)
		* game->data.perp_distance;
	sprite->casted.left_x = (WIN_WIDTH / 2) + sprite->casted.left_x
		- (sprite->casted.height / 2);
	sprite->casted.right_x = sprite->casted.left_x + sprite->casted.height;
	if (sprite->casted.right_x > WIN_WIDTH)
		sprite->casted.right_x = WIN_WIDTH;
}

void	draw_sprite(t_bonus_game *game, t_sprite *sprite)
{
	int		i;
	int		j;
	float	step;
	int		color;
	t_point	tex_px;

	step = sprite->frame_height / sprite->casted.height;
	tex_px.y = 0;
	i = sprite->casted.top_y;
	while (++i < sprite->casted.botm_y)
	{
		tex_px.y = (i + (sprite->casted.height / 2) - (WIN_HEIGHT / 2)) * step;
		j = fmaxf(sprite->casted.left_x - 1, -1);
		while (++j < sprite->casted.right_x)
		{
			tex_px.x = (j - sprite->casted.left_x) * (sprite->frame_width
					/ sprite->casted.height);
			color = get_texture_pixel(&sprite->img, tex_px.x + sprite->offset_x,
					tex_px.y);
			if (color > 0 && (j >= MINI_MAP_SIZE || i >= MINI_MAP_SIZE)
				&& sprite->casted.distance < game->distances[j])
				ft_mlx_pixel_put(&game->data.render_buf, j, i, color);
		}
	}
}

void	update_portal(t_bonus_game *game)
{
	float	distance;

	if (!game->portal.x || !game->data.player.y)
	{
		distance = fminf(game->distances[WIN_WIDTH / 2] - 10.0, 3 * CUB_SIZE);
		if (distance < 10)
			return ;
		game->portal.x = game->data.player.x + game->data.player.angle.cos
			* distance;
		game->portal.y = game->data.player.y + game->data.player.angle.sin
			* distance;
	}
	game->portal.casted.distance = get_casted_distance(game);
	teleport_player(game);
	if (game->portal.is_visible)
	{
		if (game->portal.offset_x >= game->portal.img.width)
			game->portal.offset_x = 0;
		set_sprite_dimensions(game, &game->portal);
		draw_sprite(game, &game->portal);
		if (game->portal.delta % 20 <= 0)
			game->portal.offset_x += game->portal.frame_width;
		game->portal.delta++;
	}
}
