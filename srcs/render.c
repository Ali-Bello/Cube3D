/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:38:39 by marvin            #+#    #+#             */
/*   Updated: 2024/12/15 16:38:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void    ft_mlx_pixel_put(t_img *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
    *(unsigned int *)dst = color;
}

void    draw_map(t_game *game)
{
    int i;
    int j;

    i = 0;
    while (i < 10)
    {
        j = 0;
        while (j < 10)
        {
            if (game->map[i][j] == '1')
                draw_square(game, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, 0x00110030);
            else
                draw_square(game, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, 0xFFFFFFFF);
            j++;
        }
        i++;
    }
}

void    draw_player(t_game *game)
{
    draw_circle(game, game->player.x, game->player.y,
                game->player.radius, 0x00FF0000);
}

void   draw_rectangle(t_game *game, int x, int y, int width, int height, int color)
{
    int i;
    int j;

    i = 0;
    while (i < height && y + i < game->win_height)
    {
        j = 0;
        while (j < width && x + j < game->win_width && x + j >= 0 && y + i >= 0)
        {
            ft_mlx_pixel_put(&game->render_buf, x + j, y + i, color);
            j++;
        }
        i++;
    }
}

void    clear_buf_img(t_game *game)
{
    int i;
    int j;

    i = 0;
    while (i < game->win_height)
    {
        j = 0;
        while (j < game->win_width)
        {
            ft_mlx_pixel_put(&game->render_buf, j, i, 0x00000000);
            j++;
        }
        i++;
    }
}
