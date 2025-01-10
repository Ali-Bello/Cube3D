#include "../includes/cub3d.h"

void draw_player(t_game *game)
{
    draw_circle(game, MINI_MAP_SIZE / 2 , MINI_MAP_SIZE / 2, 7.5 * MINI_MAP_SCALE_FACTOR, 0xFF0000);
}

void    rays_cast(t_game *data)
{
    int     i;
    float   angle;
    t_ray   ray;

    i = 0;
    angle = data->player.rot_angle - (FOV / 2.0);
    int offset_x = (int)(MINI_MAP_SIZE / 2 - (floorf(data->player.x) * MINI_MAP_SCALE_FACTOR));
    int offset_y = (int)(MINI_MAP_SIZE / 2 - (floorf(data->player.y)* MINI_MAP_SCALE_FACTOR));
    while (i < WIN_WIDTH)
    {
        angle = data->player.rot_angle + (atanf((i - WIN_WIDTH / 2.0) / data->plane_distance));
        cast_ray(data, &ray, angle);
        int mini_x = ray.wall_hit.x * MINI_MAP_SCALE_FACTOR + offset_x;
        int mini_y = ray.wall_hit.y * MINI_MAP_SCALE_FACTOR + offset_y;
        draw_ray(data, &ray, i);
        draw_line(data, MINI_MAP_SIZE / 2, MINI_MAP_SIZE / 2, mini_x, mini_y, 0x000FF);
        // angle += (FOV / WIN_WIDTH);
        i++;
    }
}

bool    wall_collision_check(char **map, float x, float y, float buffer)
{
    int map_x = (int)(floorf(x) / CUB_SIZE);
    int map_y = (int)(floorf(y) / CUB_SIZE);

    // Check the main position
    if (map[map_y][map_x] == '1')
        return (true);

    // Check the buffer area around the player
    if (map[(int)(floorf(y - buffer) / CUB_SIZE)][(int)(floorf(x) / CUB_SIZE)] == '1' ||
        map[(int)(floorf(y + buffer) / CUB_SIZE)][(int)(floorf(x) / CUB_SIZE)] == '1' ||
        map[(int)(floorf(y) / CUB_SIZE)][(int)(floorf(x - buffer) / CUB_SIZE)] == '1' ||
        map[(int)(floorf(y) / CUB_SIZE)][(int)(floorf(x + buffer) / CUB_SIZE)] == '1')
        return (true);

    return (false);
}
int update(t_game *data)
{
    float   move_step;
    float   next_map_player_x;
    float   next_map_player_y;
    int     x;
    int     y;
    int     dx;


    // mlx_clear_window(data->mlx, data->win);
    for (int i = 0; i < MINI_MAP_SIZE;i++)
    {
        for (int j = 0; j < MINI_MAP_SIZE;j++)
        {
            ft_mlx_pixel_put(&data->render_buf, j, i, 0x000000);
        }
    }
    mlx_mouse_get_pos(data->mlx, data->win, &x, &y);
    dx = (x - WIN_WIDTH / 2);
    if (dx != 0)
    {
        data->player.rot_angle += ROT_SPEED * dx / 10.0; // Adjust the divisor for sensitivity
        mlx_mouse_move(data->mlx, data->win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
    }
    data->last_mouse_x = WIN_WIDTH / 2;
    move_step = data->player.walk_dir * WALK_SPEED;
    next_map_player_x = data->player.x + (cosf(data->player.rot_angle) * move_step);
    next_map_player_y = data->player.y + (sinf(data->player.rot_angle) * move_step);
    data->player.rot_angle += (data->player.turn_dir * ROT_SPEED);
    if (!wall_collision_check(data->map, next_map_player_x, next_map_player_y, 10.0))
    {
        data->player.x = next_map_player_x;
        data->player.y = next_map_player_y;
    }
    else
    {
        if (!wall_collision_check(data->map, next_map_player_x, data->player.y, 10.0))
            data->player.x = next_map_player_x;
        else if (!wall_collision_check(data->map, data->player.x, next_map_player_y, 10.0))
            data->player.y = next_map_player_y;
    }
    rays_cast(data);
    draw_mini_map(data);
    draw_player(data);
    mlx_put_image_to_window(data->mlx, data->win, data->render_buf.img, 0, 0);
    return (0);
}

int main()
{
    t_game  data;

    init_game(&data);
    check_allocations(&data);
    mlx_mouse_hide(data.mlx, data.win);
    mlx_hook(data.win, 17, 0, exit_routine, &data);
    mlx_hook(data.win, 2, (1L<<0), key_press, &data);
    mlx_hook(data.win, 3, (1L<<1), key_release, &data);
    mlx_loop_hook(data.mlx, &update, &data);
    mlx_loop(data.mlx); 
    return (0);
}