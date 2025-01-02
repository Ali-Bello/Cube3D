#include "../includes/cub3d.h"

void draw_player(t_game *game)
{
    int x;
    int y;
    int offset;
    float perpendicular_angle;

    x = MINI_MAP_SIZE / 2;
    y = x;
    draw_circle(game, x, y, 10 * MINI_MAP_SCALE_FACTOR, 0xFF0000);
    int dir_x;
    int dir_y;
    dir_x = x + cosf(game->player.rot_angle) * 25 * MINI_MAP_SCALE_FACTOR;
    dir_y = y + sinf(game->player.rot_angle) * 25 * MINI_MAP_SCALE_FACTOR;
    
    // Calculate perpendicular angle
    perpendicular_angle = game->player.rot_angle + (M_PI / 2);
    // Draw player's direction lines
    for (int i = -7; i < 7; i++)
    {
        offset = i * MINI_MAP_SCALE_FACTOR;
        int start_x = x + cosf(perpendicular_angle) * offset;
        int start_y = y + sinf(perpendicular_angle) * offset;
        draw_line(game, start_x, start_y, dir_x, dir_y, 0xFF0000);
    }
}

void    rays_cast(t_game *data)
{
    int     i;
    float   angle;
    t_ray   ray;

    i = 0;
    angle = data->player.rot_angle - (FOV / 2);
    while (i < WIN_WIDTH)
    {
        cast_ray(data, &ray, angle);
        ray.plane_distance = (WIN_WIDTH / 2) / tanf(FOV / 2);
        ray.wall_height = (CUB_SIZE / (ray.distance * cosf(angle - data->player.rot_angle))) * ray.plane_distance;
        ray.top_px = (WIN_HEIGHT / 2) - (ray.wall_height / 2);
        if (ray.top_px < 0)
            ray.top_px = 0;
        ray.botm_px = ray.top_px + ray.wall_height;
        if (ray.botm_px > WIN_HEIGHT)
            ray.botm_px = WIN_HEIGHT;
        draw_ray(data, &ray, i);
        angle += FOV / WIN_WIDTH;
        i++;
    }
}

bool    wall_collision_check(char **map, float x, float y)
{
    if (map[(int)(floorf(y) / CUB_SIZE)][(int)(floorf(x) / CUB_SIZE)] == '1')
        return (true);
    return (false);
}

int update(t_game *data)
{
    float   move_step;
    float   next_map_player_x;
    float   next_map_player_y;

    mlx_clear_window(data->mlx, data->win);
    draw_map(data);
    move_step = data->player.walk_dir * WALK_SPEED;
    data->player.rot_angle += (data->player.turn_dir * ROT_SPEED);
    next_map_player_x = data->player.x + cosf(data->player.rot_angle) * move_step;
    next_map_player_y = data->player.y + sinf(data->player.rot_angle) * move_step;
    if (!wall_collision_check(data->map, next_map_player_x, next_map_player_y))
    {
        data->player.x = next_map_player_x;
        data->player.y = next_map_player_y;
    }
    rays_cast(data);
    draw_player(data);
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