/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 02:55:33 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/16 06:29:47 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void    clear_mini_map_area(t_img *render_img)
{
    int i;
    int j;

    i = -1;
    while (++i < MINI_MAP_SIZE)
    {
        j = -1;
        while (++j < MINI_MAP_SIZE)
            ft_mlx_pixel_put(render_img, j, i, 0x000000);
    }
}

void    draw_mini_ray(t_game *game, t_ray *ray)
{
    int mini_x;
    int mini_y;

    mini_x = ray->wall_hit.x * MINI_MAP_SCALE_FACTOR +\
                (int)(MINI_MAP_SIZE / 2 - (floorf(game->player.x) * MINI_MAP_SCALE_FACTOR));
    mini_y = ray->wall_hit.y * MINI_MAP_SCALE_FACTOR +\
                (int)(MINI_MAP_SIZE / 2 - (floorf(game->player.y)* MINI_MAP_SCALE_FACTOR));
    draw_line(game, MINI_MAP_SIZE / 2, MINI_MAP_SIZE / 2, mini_x, mini_y, 0x000FF); 
}

void    draw_mini_map(t_game *game)
{
    t_point offset;
    int scaled_size;
    int i;
    int j;

    offset.x = MINI_MAP_SIZE / 2 - (game->player.x * MINI_MAP_SCALE_FACTOR);
    offset.y = MINI_MAP_SIZE / 2 - (game->player.y * MINI_MAP_SCALE_FACTOR);
    scaled_size = CUB_SIZE * MINI_MAP_SCALE_FACTOR + 1;
    i = -1;
    while (game->map[++i])
    {
        j = -1;
        while (game->map[i][++j])
        {
            if (game->map[i][j] == '1')
                draw_square(game, (j * CUB_SIZE * MINI_MAP_SCALE_FACTOR) + offset.x,
                (i * CUB_SIZE * MINI_MAP_SCALE_FACTOR) + offset.y,
                scaled_size, 0xAAAAAAA);
            else if (game->map[i][j] == 'D')
                draw_square(game, (j * CUB_SIZE * MINI_MAP_SCALE_FACTOR) + offset.x,
                (i * CUB_SIZE * MINI_MAP_SCALE_FACTOR) + offset.y,
                scaled_size, 0x00ccFF);
        }
    }
    draw_mini_sprite(game, offset);
    draw_circle(game, MINI_MAP_SIZE / 2 , MINI_MAP_SIZE / 2, 7.5 * MINI_MAP_SCALE_FACTOR, 0xFF0000);
}

void draw_mini_sprite(t_game *game, t_point offset)
{
    if (!game->spawn_portal)
        return ;
    game->portal.angle = game->player.angle.rad - 
                        atan2f(game->portal.y - game->player.y, game->portal.x - game->player.x);
    game->portal.angle = normalize_angle(game->portal.angle);
    game->portal.angle = fabsf(game->portal.angle);
    if (game->portal.angle < FOV / 2 || game->portal.angle > (2 * M_PI - FOV / 2))
    {
        game->portal.is_visible = true;
        draw_square(game, game->portal.x * MINI_MAP_SCALE_FACTOR + offset.x, 
                   game->portal.y * MINI_MAP_SCALE_FACTOR + offset.y, 5, 0x00AAFF);
    }
    else
    {
        draw_square(game, game->portal.x * MINI_MAP_SCALE_FACTOR + offset.x,
                   game->portal.y * MINI_MAP_SCALE_FACTOR + offset.y, 5, 0x807e7a);
        game->portal.is_visible = false;
    }
}
