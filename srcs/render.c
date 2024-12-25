/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:38:39 by marvin            #+#    #+#             */
/*   Updated: 2024/12/21 14:33:01 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	ft_cpy_pixel(t_img *src, t_img *dst, int x, int y)
{
    char    *dst_px;
    char    *src_px;

    // printf("y = [%d], x = [%d]\n", y * src->line_len % TILE_SIZE, 9);
    dst_px = dst->addr + ((y * dst->line_len) + (x * (dst->bpp / 8)));
    src_px = src->addr + ((y * src->line_len % TILE_SIZE) + (x % TILE_SIZE * src->bpp / 8));
    *(unsigned int *)dst_px = *(unsigned int *)src_px;
}

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
    while (game->map[i])
    {
        j = 0;
        while (game->map[i][j])
        {
            if (game->map[i][j] == '1')
                draw_square(game, j * TILE_SIZE * SCALE_FACTOR, i * TILE_SIZE
                * SCALE_FACTOR, TILE_SIZE * SCALE_FACTOR, 0x00110030);
            else
                draw_square(game, j * TILE_SIZE * SCALE_FACTOR, i * TILE_SIZE
                * SCALE_FACTOR, TILE_SIZE * SCALE_FACTOR, 0xFFFFFFFF);
            j++;
        }
        i++;
    }
}

void    draw_player(t_game *game)
{
    draw_circle(game, game->player.x * SCALE_FACTOR, game->player.y * SCALE_FACTOR,
                game->player.radius * SCALE_FACTOR, 0x00FF0000);
}

void    draw_ray(t_game *game, t_ray *ray, int w_idx)
{
    int i;

    i = 0;
    while (i < ray->top_px)
    {
        // ft_cpy_pixel(&game->textures, &game->render_buf, w_idx, i);
        ft_mlx_pixel_put(&game->render_buf, w_idx, i, 0xFF3333);
        i++;
    }
    i = ray->top_px;
    while (i < ray->botm_px)
    {
        ft_cpy_pixel(&game->textures, &game->render_buf, w_idx, i);
        // ft_mlx_pixel_put(&game->render_buf, w_idx, i, 0xAAAAAAA);
        i++;
    }
    i = ray->botm_px;
    while (i < game->win_height)
    {
        // ft_mlx_pixel_put(&game->render_buf, w_idx, i, 0x000000);
        i++;
    }
}

void   draw_rectangle(t_game *game, int x, int y, int width, int height, int color)
{
    int i;
    int j;

    i = 0;
    while (i < y)
        ft_mlx_pixel_put(&game->render_buf, x, i++, 0x00110030);
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
    i = y + height;
    while (i < game->win_height)
        ft_mlx_pixel_put(&game->render_buf, x, i++, 0x002230);
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
