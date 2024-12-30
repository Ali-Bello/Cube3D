#include "../includes/cub3d.h"

void    rays_cast(t_game *data)
{
    int     i;
    float   angle;
    t_ray   ray;

    i = 0;
    angle = data->player.rot_angle - (FOV / 2);
    while (i < data->win_width)
    {
        cast_ray(data, &ray, angle);
        ray.plane_distance = (data->win_width / 2) / tanf(FOV / 2);
        ray.wall_height = (TILE_SIZE / (ray.distance * cosf(angle - data->player.rot_angle))) * ray.plane_distance;
        ray.top_px = (data->win_height / 2) - (ray.wall_height / 2);
        if (ray.top_px < 0)
            ray.top_px = 0;
        ray.botm_px = (data->win_height / 2) + (ray.wall_height / 2);
        if (ray.botm_px > data->win_height)
            ray.botm_px = data->win_height;
        draw_ray(data, &ray, i);
        // draw_line(data,
        //     data->player.x * SCALE_FACTOR,
        //     data->player.y * SCALE_FACTOR,
        //     ray.wall_hit.x * SCALE_FACTOR,
        //     ray.wall_hit.y * SCALE_FACTOR,
        //     0x003115);
        angle += FOV / data->win_width;
        i++;
    }
}

bool    wall_collision_check(char **map, float x, float y)
{
    if (map[(int)(floorf(y) / TILE_SIZE)][(int)(floorf(x) / TILE_SIZE)] == '1')
        return (true);
    return (false);
}

int update(t_game *data)
{
    float   move_step;
    float   next_map_player_x;
    float   next_map_player_y;

    mlx_clear_window(data->mlx, data->win);
    // draw_map(data);
    move_step = data->player.walk_dir * data->player.mov_speed;
    data->player.rot_angle += (data->player.turn_dir * data->player.rot_speed);
    next_map_player_x = data->player.x + cosf(data->player.rot_angle) * move_step;
    next_map_player_y = data->player.y + sinf(data->player.rot_angle) * move_step;
    if (!wall_collision_check(data->map, next_map_player_x, next_map_player_y))
    {
        data->player.x = next_map_player_x;
        data->player.y = next_map_player_y;
    }
    draw_player(data);
    rays_cast(data);
    mlx_put_image_to_window(data->mlx, data->win, data->render_buf.img, 0, 0);
    return (0);
}

int main()
{
    t_game  data;

    init_game(&data);
    check_allocations(&data);
    mlx_hook(data.win, 17, 0, exit_routine, &data);
    mlx_hook(data.win, 2, (1L<<0), key_press, &data);
    mlx_hook(data.win, 3, (1L<<1), key_release, &data);
    mlx_loop_hook(data.mlx, &update, &data);
    mlx_loop(data.mlx); 
    return (0);
}