#include "../includes/cub3d.h"

void    draw_square(t_game *game, int x, int y, int size, int color)
{
    int i;
    int j;

    i = 0;
    while (i < size)
    {
        j = 0;
        while (j < size)
        {
            if (i == 0 || j == 0)
                ft_mlx_pixel_put(&game->render_buf, x + j, y + i, 0x000000);
            else
                ft_mlx_pixel_put(&game->render_buf, x + j, y + i, color);
            j++;
        }
        i++;
    }
}

void    draw_circle(t_game *game, int x, int y, int radius, int color)
{
    int i;
    int j;

    i = -radius;
    while (i < radius)
    {
        j = -radius;
        while (j < radius)
        {
            if ((i * i) + (j * j) < radius * radius)
                ft_mlx_pixel_put(&game->render_buf, x + j, y + i, color);
            j++;
        }
        i++;
    }
}

void draw_line(t_game *game, int x, int y, int color)
{
    int dx, dy, p, x0, y0, x1, y1, x_inc, y_inc;

    x0 = game->player.x;
    y0 = game->player.y;
    x1 = x;
    y1 = y;

    if (x < 0)
        x1 = 0;
    else if (x >= game->win_width)
        x1 = game->win_width - 1;
    if (y < 0)
        y1 = 0;
    else if (y >= game->win_height)
        y1 = game->win_height - 1;
    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    x_inc = (x0 < x1) ? 1 : -1;
    y_inc = (y0 < y1) ? 1 : -1;

    if (dx > dy)
    {
        p = 2 * dy - dx;
        while (x0 != x1)
        {
            ft_mlx_pixel_put(&game->render_buf, x0, y0, color);
            x0 += x_inc;
            if (p >= 0)
            {
                y0 += y_inc;
                p -= 2 * dx;
            }
            p += 2 * dy;
        }
    }
    else
    {
        p = 2 * dx - dy;
        while (y0 != y1)
        {
            ft_mlx_pixel_put(&game->render_buf, x0, y0, color);
            y0 += y_inc;
            if (p >= 0)
            {
                x0 += x_inc;
                p -= 2 * dy;
            }
            p += 2 * dx;
        }
    }
    ft_mlx_pixel_put(&game->render_buf, x0, y0, color);
}
