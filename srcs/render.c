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

void	ft_cpy_wall_px(t_img *src, t_img *dst, t_point *src_pos, t_point *dst_pos)
{
    char    *dst_px;
    char    *src_px;

    if (src_pos->x < 0 || src_pos->x >= src->width || src_pos->y < 0 || src_pos->y >= src->height)
        return;
    if (dst_pos->x < 0 || dst_pos->x >= dst->width || dst_pos->y < 0 || dst_pos->y >= dst->height)
        return;
    dst_px = dst->addr + ((int)dst_pos->y * dst->line_len + (int)(dst_pos->x * (dst->bpp / 8)));
    src_px = &src->addr[(int)src_pos->y * src->line_len + (int)(src_pos->x * (src->bpp / 8))];
        memcpy(dst_px, src_px, src->bpp / 8);
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
    t_point render_cord;
    t_point wall_cord;
    int     i;

    i = 0;
    render_cord.x = w_idx;
    if (ray->vert_wall_hit.is_hit)
        wall_cord.x = (int)ray->wall_hit.y % TILE_SIZE;
    else
        wall_cord.x = (int)ray->wall_hit.x % TILE_SIZE;
    while (i < ray->top_px)
    {
        // ft_cpy_pixel(&game->textures, &game->render_buf, w_idx, i);
        ft_mlx_pixel_put(&game->render_buf, w_idx, i, 0xFF3333);
        i++;
    }
    i = ray->top_px;
    while (i < ray->botm_px)
    {
        render_cord.y = i;
        wall_cord.y = (i - ray->top_px) * game->textures.height / ray->wall_height;
        ft_cpy_wall_px(&game->textures, &game->render_buf, &wall_cord, &render_cord);
        // ft_mlx_pixel_put(&game->render_buf, w_idx, i, 0xAAAAAAA);
        i++;
    }
    i = ray->botm_px;
    while (i < game->win_height)
    {
        ft_mlx_pixel_put(&game->render_buf, w_idx, i, 0x424242);
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
