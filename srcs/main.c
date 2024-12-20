#include "../includes/cub3d.h"

char    **create_map()
{
    char    **map;

    map = calloc(14, sizeof(char *));
    map[0] = strdup("1111111111111111111");
    map[1] = strdup("1000000000000000011");
    map[2] = strdup("1000111000000000011");
    map[3] = strdup("1000000000000000011");
    map[4] = strdup("1010000100000000011");
    map[5] = strdup("1010000100000000011");
    map[6] = strdup("1000100000000001111");
    map[7] = strdup("1000110000000000011");
    map[8] = strdup("1000100000000000011");
    map[9] = strdup("1000000000000000011");
    map[10] = strdup("1000000000000000101");
    map[11] = strdup("1000000000000000101");
    map[12] = strdup("111111111111111111");
    map[13] = NULL;
    return (map);
}

void    rays_cast(t_game *data)
{
    int     i;
    float   ray_angle;
    float   plane_distance;
    float   ray_height;
    t_ray   ray;

    i = 0;
    ray_angle = data->player.rot_angle - (FOV / 2);
    while (i < data->win_width)
    {
        cast_ray(data, &ray, ray_angle);
        plane_distance = (data->win_width / 2) / tanf(FOV / 2);
        ray_height = (TILE_SIZE / ray.distance) * plane_distance;
        draw_rectangle(data, i, (data->win_height / 2) - (ray_height / 2), 1, ray_height, 0x00FF0000);
        // draw_line(data, ray.wall_hit.x, ray.wall_hit.y, 0x00FF0000);
        ray_angle += FOV / (640);
        i++;
    }
}

int update(t_game *data)
{
    float     move_step;
    float     next_map_player_x;
    float    next_map_player_y;

    mlx_clear_window(data->mlx, data->win);
    clear_buf_img(data);
    // draw_map(data);
    move_step = data->player.walk_dir * data->player.mov_speed;
    data->player.rot_angle += (data->player.turn_dir * data->player.rot_speed);
    next_map_player_x = data->player.x + cosf(data->player.rot_angle) * move_step;
    next_map_player_y = data->player.y + sinf(data->player.rot_angle) * move_step;
    if (data->map[(int)floor(next_map_player_y) / TILE_SIZE]\
    [(int)floor(next_map_player_x) / TILE_SIZE] != '1')   
    {
        data->player.x = next_map_player_x;
        data->player.y = next_map_player_y;
    }
    // draw_player(data);
    rays_cast(data);
    mlx_put_image_to_window(data->mlx, data->win, data->render_buf.img, 0, 0);
    return (0);
}

int main()
{
    t_game  data;

    memset(&data, 0, sizeof(t_game));
    data.map = create_map();
    data.win_width = strlen(data.map[0]) * TILE_SIZE;
    data.win_height = 13 * TILE_SIZE;
    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, data.win_width, data.win_height, "Cube3D");
    data.render_buf.img = mlx_new_image(data.mlx, data.win_width, data.win_height);
    data.render_buf.addr = mlx_get_data_addr(data.render_buf.img, &data.render_buf.bpp,
                        &data.render_buf.line_len, &data.render_buf.endian);
    data.player.x = TILE_SIZE + 32;
    data.player.y = TILE_SIZE + 32;
    data.player.radius = 8;
    data.player.rot_angle = M_PI / 2;
    data.player.mov_speed = 3;
    data.player.rot_speed = 3 *(M_PI / 180);
    mlx_hook(data.win, 17, 0, exit_routine, &data);
    mlx_hook(data.win, 2, (1L<<0), key_press, &data);
    mlx_hook(data.win, 3, (1L<<1), key_release, &data);
    mlx_loop_hook(data.mlx, &update, &data);
    mlx_loop(data.mlx); 
    return (0);
}