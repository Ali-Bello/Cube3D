#include "../includes/cub3d.h"

void draw_square(t_game *game, int x, int y, int size, int color)
{
    int i, j;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (x + i >= 0 && x + i <= MINI_MAP_SIZE && y + j >= 0 && y + j <= MINI_MAP_SIZE)
            {
                ft_mlx_pixel_put(&game->render_buf, x + i, y + j, color);
            }
        }
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

void draw_line(t_game *game, int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        ft_mlx_pixel_put(&game->render_buf, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = err * 2;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}
