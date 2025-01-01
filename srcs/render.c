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

void    ft_cpy_wall_px(t_img *src, t_img *dst, t_point *src_pos, t_point *dst_pos) 
{
    char *dst_px;
    char *src_px;

    dst_px = dst->addr + ((int)(dst_pos->y * dst->line_len) + (int)(dst_pos->x * (dst->bpp / 8)));
    src_px = &src->addr[(int)(src_pos->y * src->line_len) + (int)(src_pos->x * (src->bpp / 8))];
    memcpy(dst_px, src_px, src->bpp / 8);
}

void    ft_mlx_pixel_put(t_img *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
    *(unsigned int *)dst = color;
}
void draw_map(t_game *game)
{
    int x, y;

    int offset_x = MINI_MAP_SIZE / 2 - (game->player.x * MINI_MAP_SCALE_FACTOR);
    int offset_y = MINI_MAP_SIZE / 2 - (game->player.y * MINI_MAP_SCALE_FACTOR);

    for (y = 0; game->map[y]; y++)
    {
        for (x = 0; x < game->map[y][x] ; x++)
        {
            int scaled_x = x * TILE_SIZE * MINI_MAP_SCALE_FACTOR + offset_x;
            int scaled_y = y * TILE_SIZE * MINI_MAP_SCALE_FACTOR + offset_y;
            int scaled_tile_size = TILE_SIZE * MINI_MAP_SCALE_FACTOR;

            if (game->map[y][x] == '1')
            {
                // Draw wall tile
                draw_square(game, scaled_x, scaled_y, scaled_tile_size, 0xAAAAAAA);
            }
            else
            {
                // Draw empty tile
                draw_square(game, scaled_x, scaled_y, scaled_tile_size, 0x000000);
            }
        }
    }
}


void    draw_ray(t_game *game, t_ray *ray, int w_idx)
{
    t_point render_cord;
    t_point wall_cord;
    int     i;

    i = 0;
    render_cord.x = w_idx;
    if (ray->wall_hit_face)
        wall_cord.x = (int)ray->wall_hit.y % game->textures.width;
    else
        wall_cord.x = (int)ray->wall_hit.x % game->textures.width;
    while (i < ray->top_px)
    {
        if (i > 150 || w_idx > 150)
            ft_mlx_pixel_put(&game->render_buf, w_idx, i, 0xFF3333);
        i++;
    }
    i = ray->top_px;
    while (i < ray->botm_px)
    {
        render_cord.y = i;//img
        wall_cord.y = (i + (ray->wall_height / 2 -  WIN_HEIGHT / 2)) * game->textures.height / ray->wall_height;// texture
        if (i > 150 || w_idx > 150)
            ft_cpy_wall_px(&game->textures, &game->render_buf, &wall_cord, &render_cord);//copies
        i++;
    }
    i = ray->botm_px;
    while (i < WIN_HEIGHT)
    {
        ft_mlx_pixel_put(&game->render_buf, w_idx, i, 0x4242AA);
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
    while (i < WIN_HEIGHT)
    {
        j = 0;
        while (j < WIN_WIDTH)
        {
            ft_mlx_pixel_put(&game->render_buf, j, i, 0x00000000);
            j++;
        }
        i++;
    }
}
