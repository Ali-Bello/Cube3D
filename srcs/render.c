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

void    ft_mlx_pixel_put(t_img *data, int x, int y, int color)
{
    char    *dst;

    if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
        return ;
    dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
    *(unsigned int *)dst = color;
}

int get_texture_pixel(t_img *texture, int x, int y)
{
	int		pixel_color;
	char	*pixel;

    if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
        return (0);
    pixel = texture->addr + (y * texture->line_len + x * (texture->bpp / 8));
    pixel_color = *(int *)pixel;
	return (pixel_color);
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
        }
    }
}

int get_texture_id(t_game *game, t_ray *ray)
{
    int tex_id;

    if (game->map[(int)(ray->wall_hit.y / CUB_SIZE)]\
    [(int)(ray->wall_hit.x / CUB_SIZE)] == 'D')
        return (4);
    if (!ray->wall_hit_face)
    {
        if (!ray->facing_left)
            tex_id = 0; // east
        else
            tex_id = 1; // west
    }
    else
    {
        if (!ray->facing_down)
            tex_id = 2; // south
        else
            tex_id = 3; // north
    }
    return (tex_id);
}

void    draw_bounds_line(t_game *game, int start, int end, int x, int color)
{
    while (start < end)
    {
        if (start >= MINI_MAP_SIZE || x >= MINI_MAP_SIZE)
            ft_mlx_pixel_put(&game->render_buf, x, start, color);
        start++;
    }
}
float   set_texture_cordinates(t_ray *ray, t_img *texture, t_point *cords)
{
    float step;

    if (ray->wall_hit_face) 
        cords->x = (fmodf(ray->wall_hit.x, CUB_SIZE) / CUB_SIZE) * texture->width;
    else
        cords->x = (fmodf(ray->wall_hit.y, CUB_SIZE) / CUB_SIZE) * texture->width;
    cords->y = 0;
    step = (float)texture->height / (float)ray->wall_height;
    if (ray->botm_px - ray->top_px > WIN_HEIGHT)
    {
        cords->y = (float)(0 - ray->top_px) * step;
        ray->top_px = 0;
    }
    if (ray->botm_px > WIN_HEIGHT)
        ray->botm_px = WIN_HEIGHT;
    return (step);
}

int shade_color(int color, float distance, bool is_vertical)
{
    int r;
    int g;
    int b;
    float shade_factor;
    
    shade_factor = fminf(1.0, 1.0 / (distance * 0.009));
    
    if (is_vertical)
        shade_factor *= 0.85;

    r = ((color >> 16) & 0xFF) * shade_factor;
    g = ((color >> 8) & 0xFF) * shade_factor;
    b = (color & 0xFF) * shade_factor;

    return ((r << 16) | (g << 8) | b);
}

void draw_ray(t_game *game, t_ray *ray, int w_idx)
{
    t_point tex_cord;
    t_img   *texture;
    int     i;
    float   step;
    int     color;

    texture = &game->textures[get_texture_id(game, ray)];
    draw_bounds_line(game, 0, ray->top_px, w_idx, 0x4242AA);
    step = set_texture_cordinates(ray, texture, &tex_cord);
    i = ray->top_px;

    while (i < ray->botm_px)
    {
        if (i >= MINI_MAP_SIZE || w_idx >= MINI_MAP_SIZE)
        {
            color = get_texture_pixel(texture, tex_cord.x, tex_cord.y);
            color = shade_color(color, ray->distance, ray->wall_hit_face);
            ft_mlx_pixel_put(&game->render_buf, w_idx, i, color);
        }
        tex_cord.y += step;
        i++;
    }
    draw_bounds_line(game, ray->botm_px, WIN_HEIGHT, w_idx, 0x42AA6B);
}

