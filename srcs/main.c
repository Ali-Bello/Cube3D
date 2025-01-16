#include "../includes/cub3d.h"

void    rays_cast(t_game *game)
{
    int     i;  
    float   angle;
    t_ray   ray;

    i = 0;
    angle = game->player.angle.rad - (FOV / 2.0);
    while (i < WIN_WIDTH)
    {
        angle = game->player.angle.rad + (atan2f((i - WIN_WIDTH / 2.0) , game->perp_distance));
        cast_ray(game, &ray, angle);
        game->distances[i] = ray.distance;
        draw_ray(game, &ray, i);
        draw_mini_ray(game, &ray);
        i++;
    }
}

bool    check_surrondings(char **map, float x, float y, char c)
{
    if (map[(int)((y - 10.0) / CUB_SIZE)][(int)((x) / CUB_SIZE)] == c ||
        map[(int)((y + 10.0) / CUB_SIZE)][(int)((x) / CUB_SIZE)] == c ||
        map[(int)((y) / CUB_SIZE)][(int)((x - 10.0) / CUB_SIZE)] == c ||
        map[(int)((y) / CUB_SIZE)][(int)((x + 10.0) / CUB_SIZE)] == c)
        return (true);
    return (false);
}

bool    wall_collision_check(t_game *game, float x, float y)
{
    int map_x;
    int map_y;
    
    map_x = (int)(floorf(x) / CUB_SIZE);
    map_y = (int)(floorf(y) / CUB_SIZE);
    if (game->map[map_y][map_x] == '1'
        || map_x < 0 || map_y < 0)
        return (true);
    if (!game->door_open && check_surrondings(game->map, x, y, 'D'))
        return (true);
    return (check_surrondings(game->map, x, y, '1'));
}

void    update_mouse_interaction(t_game *game)
{
    int     x;
    int     y;
    int     dx;

    mlx_mouse_get_pos(game->mlx, game->win, &x, &y);
    dx = (x - WIN_WIDTH / 2);
    if (dx != 0)
    {
        game->player.angle.rad += ROT_SPEED * dx / 10.0;
        mlx_mouse_move(game->mlx, game->win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
    }
    game->last_mouse_x = WIN_WIDTH / 2;
}



void    update_player_position(t_game *game)
{
    float   move_step;
    float   strafe_step;
    float   next_map_player_x;
    float   next_map_player_y;

    move_step = game->player.walk_dir * WALK_SPEED;
    strafe_step = game->player.strafe_dir * WALK_SPEED;
    next_map_player_x = game->player.x + (game->player.angle.cos * move_step) + (cosf(game->player.angle.rad + M_PI_2) * strafe_step);
    next_map_player_y = game->player.y + (game->player.angle.sin * move_step) + (sinf(game->player.angle.rad + M_PI_2) * strafe_step);
    
    // next_map_player_x = game->player.x + (game->player.angle.cos * move_step);
    // next_map_player_y = game->player.y + (game->player.angle.sin * move_step);
    
    game->player.angle.rad += (game->player.turn_dir * ROT_SPEED);
    if (!wall_collision_check(game, next_map_player_x, next_map_player_y))
    {
        game->player.x = next_map_player_x;
        game->player.y = next_map_player_y;
    }
    else
    {
        if (!wall_collision_check(game, next_map_player_x, game->player.y))
            game->player.x = next_map_player_x;
        else if (!wall_collision_check(game, game->player.x, next_map_player_y))
            game->player.y = next_map_player_y;
    }
}

int update(t_game *game)
{
    game->player.angle.rad = normalize_angle(game->player.angle.rad);
    game->player.angle.cos = cosf(game->player.angle.rad);
    game->player.angle.sin = sinf(game->player.angle.rad);
    game->player.angle.tan = tanf(game->player.angle.rad);
    clear_mini_map_area(&game->render_buf);
    update_mouse_interaction(game);
    update_player_position(game);
    rays_cast(game);
    draw_mini_map(game);
    if (game->spawn_portal)
        update_portal(game);
    mlx_put_image_to_window(game->mlx, game->win, game->render_buf.img, 0, 0);
    return (0);
}

int main()
{
    t_game  data;

    srand(time(NULL)); 
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