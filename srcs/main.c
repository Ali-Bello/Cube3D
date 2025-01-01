#include "../includes/cub3d.h"

void draw_mini_ray(t_game *game, t_ray *ray)
{
    float   scale_factor;
    int     px;
    int     py;

    scale_factor = fminf((float)MINI_MAP_SIZE / (game->win_width * TILE_SIZE),\
                    (float)MINI_MAP_SIZE / (game->win_height * TILE_SIZE));
    px = (game->player.x / TILE_SIZE) * scale_factor;
    py = (game->player.y / TILE_SIZE) * scale_factor;
    (void)ray;
    printf("px = [%d], py = [%d]\n", px, py);
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
        ray.wall_height = (TILE_SIZE / (ray.distance * cosf(angle - data->player.rot_angle))) * ray.plane_distance;
        ray.top_px = (WIN_HEIGHT / 2) - (ray.wall_height / 2);
        if (ray.top_px < 0)
            ray.top_px = 0;
        ray.botm_px = (WIN_HEIGHT / 2) + (ray.wall_height / 2);
        if (ray.botm_px > WIN_HEIGHT)
            ray.botm_px = WIN_HEIGHT;
        draw_ray(data, &ray, i);
        // draw_mini_ray(data, &ray);
        angle += FOV / WIN_WIDTH;
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