/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 01:48:42 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/16 06:19:18 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void    teleport_player(t_game *game)
{
    int new_x;
    int new_y;

    if (game->portal.casted.distance <= 10.0 && game->portal.is_visible)
    {
        game->spawn_portal = false;
        new_x = (rand() % game->map_width) * CUB_SIZE;
        new_y = (rand() % game->map_height) * CUB_SIZE;
        while (new_y / CUB_SIZE >= game->map_height ||
        new_x / CUB_SIZE >= game->map_width ||
        wall_collision_check(game, new_x, new_y))
        {
            new_x = rand() % game->map_width * CUB_SIZE;
            new_y = rand() % game->map_height * CUB_SIZE;
        }
        game->player.x = new_x;
        game->player.y = new_y;
        game->portal.x = 0;
        game->portal.y = 0;
    }
}

void    set_sprite_dimensions(t_game *game, t_sprite  *sprite)
{
    sprite->casted.height = (CUB_SIZE / sprite->casted.distance) * game->perp_distance;
    sprite->casted.top_y = (WIN_HEIGHT - sprite->casted.height) / 2.0;
    sprite->casted.botm_y = sprite->casted.top_y + sprite->casted.height;
    if (sprite->casted.botm_y - sprite->casted.top_y > WIN_HEIGHT)
        sprite->casted.top_y = 0;
    if (sprite->casted.botm_y > WIN_HEIGHT)
        sprite->casted.botm_y = WIN_HEIGHT;
    sprite->casted.left_x = tanf(atan2f(sprite->y - game->player.y,\
        sprite->x - game->player.x) - game->player.angle.rad) * game->perp_distance;
    sprite->casted.left_x = (WIN_WIDTH / 2) + sprite->casted.left_x - (sprite->casted.height / 2);
    sprite->casted.right_x = sprite->casted.left_x + sprite->casted.height; // width == height because it's a sprite frame
    if (sprite->casted.right_x > WIN_WIDTH)
        sprite->casted.right_x = WIN_WIDTH;
}

void    draw_portal(t_game *game)
{
    int i;
    int j;
    float step;
    int color;
    t_point tex_px;

    step = game->portal.frame_height / game->portal.casted.height;
    tex_px.y = 0;
    i = game->portal.casted.top_y - 1;
    while (++i < game->portal.casted.botm_y)
    {
        tex_px.y = (i + (game->portal.casted.height / 2) - (WIN_HEIGHT / 2)) * step;
        j = (game->portal.casted.left_x - 1);
        if (j < 0)
            j = -1;
        while (++j < game->portal.casted.right_x)
        {
            tex_px.x = (j - game->portal.casted.left_x) * (game->portal.frame_width / game->portal.casted.height);
            color = get_texture_pixel(&game->portal.img, tex_px.x + game->portal.offset_x, tex_px.y);
            if (color >= 0 && (j >= MINI_MAP_SIZE || i >= MINI_MAP_SIZE)
                && game->portal.casted.distance < game->distances[j])
                    ft_mlx_pixel_put(&game->render_buf, j, i, color);
        }
    }
}

void    update_portal(t_game *game)
{
    static int delta;
    float portal_distance;

    if (!game->portal.x || !game->player.y)
    {
        portal_distance = fminf(game->distances[WIN_WIDTH / 2] - 10.0, 3 * CUB_SIZE);
        game->portal.x = game->player.x + game->player.angle.cos * portal_distance;
        game->portal.y = game->player.y + game->player.angle.sin * portal_distance;
    }
    game->portal.casted.distance = distance_from_origin(&game->player, game->portal.x, game->portal.y) *
                        cosf(atan2f(game->portal.y - game->player.y, game->portal.x - game->player.x) - game->player.angle.rad);
    teleport_player(game);
    if (game->portal.is_visible)
    {
        if (game->portal.offset_x >= game->portal.img.width)
            game->portal.offset_x = 0;
        set_sprite_dimensions(game, &game->portal);
        draw_portal(game);
        if (delta % 30 == 0)
            game->portal.offset_x += game->portal.frame_width;
        delta++;
    }
}
