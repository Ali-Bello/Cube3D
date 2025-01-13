#include "../includes/cub3d.h"

void draw_player(t_game *game)
{
    draw_circle(game, MINI_MAP_SIZE / 2 , MINI_MAP_SIZE / 2, 7.5 * MINI_MAP_SCALE_FACTOR, 0xFF0000);
}

void    draw_mini_ray(t_game *game, t_ray *ray)
{
    int mini_x;
    int mini_y;

    mini_x = ray->wall_hit.x * MINI_MAP_SCALE_FACTOR +\
                (int)(MINI_MAP_SIZE / 2 - (floorf(game->player.x) * MINI_MAP_SCALE_FACTOR));
    mini_y = ray->wall_hit.y * MINI_MAP_SCALE_FACTOR +\
                (int)(MINI_MAP_SIZE / 2 - (floorf(game->player.y)* MINI_MAP_SCALE_FACTOR));
    draw_line(game, MINI_MAP_SIZE / 2, MINI_MAP_SIZE / 2, mini_x, mini_y, 0x000FF);
}

void draw_sprite(t_game *game)
{
    float   distance;
    float   sprite_height;
    float   top_y;
    float   botm_y;
    float   sprite_x;
    float   left_x;
    float   right_x;
    int     width;
    int     height;
    int     color;
    float   step;
    t_point tex_cord;

    distance = distance_from_origin(&game->player, 192, 300) * cosf(game->sprite.angle);
    sprite_height = (CUB_SIZE / distance) * game->plane_distance;
    top_y = (WIN_HEIGHT - sprite_height) / 2.0;
    botm_y = top_y + sprite_height;
    if (botm_y - top_y > WIN_HEIGHT)
        top_y = 0;
    if (botm_y > WIN_HEIGHT)
        botm_y = WIN_HEIGHT;
    sprite_x = tanf(atan2f(300 - game->player.y, 192 - game->player.x) - game->player.rot_angle) * game->plane_distance;
    left_x = (WIN_WIDTH / 2) + sprite_x - (sprite_height / 2);
    right_x = left_x + sprite_height;
    if (right_x > WIN_WIDTH)
        right_x = WIN_WIDTH;
    int start_x = left_x;
    if (left_x < 0)
        start_x = 0;
    // printf("left_px = [%d], right_px = [%f]\n", start_x, right_x);
    static int offset;
    static int z;
    if (offset >= 640)
        offset = 0;
    if (game->sprite.is_visible)
    {
        width = 128;
        height = 120;
        step = (float)height / sprite_height;
        tex_cord.y = 0;

        for (int i = top_y; i < botm_y; i++)
        {
            int dy = i + (sprite_height / 2) - (WIN_HEIGHT / 2);
            tex_cord.y = dy * step;
            for (int j = start_x; j < right_x; j++)
            {
                tex_cord.x = (j - left_x) * (width / sprite_height);
                color = get_texture_pixel(&game->sprite.img, tex_cord.x + offset, tex_cord.y);
                if (color >= 0 && (j >= MINI_MAP_SIZE || i >= MINI_MAP_SIZE))
                {
                    // Cast a ray for the current column to get the wall distance
                    t_ray ray;
                    float angle = game->player.rot_angle + atan2f((j - WIN_WIDTH / 2.0), game->plane_distance);
                    cast_ray(game, &ray, angle);
                    // Depth check
                    if (distance < ray.distance)
                        ft_mlx_pixel_put(&game->render_buf, j, i, color);
                }
            }
        }
    }
    if (z % 50 == 0)
        offset += 128;
    z++;
}

void    rays_cast(t_game *game)
{
    int     i;
    float   angle;
    t_ray   ray;

    i = 0;
    angle = game->player.rot_angle - (FOV / 2.0);
    while (i < WIN_WIDTH)
    {
        angle = game->player.rot_angle + (atan2f((i - WIN_WIDTH / 2.0) , game->plane_distance));
        cast_ray(game, &ray, angle);
        draw_ray(game, &ray, i);
        draw_mini_ray(game, &ray);
        i++;
    }
}

bool    wall_collision_check(char **map, float x, float y, float buffer)
{
    int map_x;
    int map_y;

    map_x = (int)(floorf(x) / CUB_SIZE);
    map_y = (int)(floorf(y) / CUB_SIZE);
    if (map[map_y][map_x] == '1')
        return (true);
    if (map[(int)(floorf(y - buffer) / CUB_SIZE)][(int)(floorf(x) / CUB_SIZE)] == '1' ||
        map[(int)(floorf(y + buffer) / CUB_SIZE)][(int)(floorf(x) / CUB_SIZE)] == '1' ||
        map[(int)(floorf(y) / CUB_SIZE)][(int)(floorf(x - buffer) / CUB_SIZE)] == '1' ||
        map[(int)(floorf(y) / CUB_SIZE)][(int)(floorf(x + buffer) / CUB_SIZE)] == '1')
        return (true);
    return (false);
}

void    clear_mini_map_area(t_img *render_img)
{
    int i;
    int j;

    i = -1;
    while (++i < MINI_MAP_SIZE)
    {
        j = -1;
        while (++j < MINI_MAP_SIZE)
            ft_mlx_pixel_put(render_img, j, i, 0x000000);
    }
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
        game->player.rot_angle += ROT_SPEED * dx / 10.0;
        mlx_mouse_move(game->mlx, game->win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
    }
    game->last_mouse_x = WIN_WIDTH / 2;
}

void    update_player_position(t_game *game)
{
    float   move_step;
    float   next_map_player_x;
    float   next_map_player_y;

    move_step = game->player.walk_dir * WALK_SPEED;
    next_map_player_x = game->player.x + (cosf(game->player.rot_angle) * move_step);
    next_map_player_y = game->player.y + (sinf(game->player.rot_angle) * move_step);
    game->player.rot_angle += (game->player.turn_dir * ROT_SPEED);
    if (!wall_collision_check(game->map, next_map_player_x, next_map_player_y, 10.0))
    {
        game->player.x = next_map_player_x;
        game->player.y = next_map_player_y;
    }
    else
    {
        if (!wall_collision_check(game->map, next_map_player_x, game->player.y, 10.0))
            game->player.x = next_map_player_x;
        else if (!wall_collision_check(game->map, game->player.x, next_map_player_y, 10.0))
            game->player.y = next_map_player_y;
    }
}

int update(t_game *game)
{
    clear_mini_map_area(&game->render_buf);
    update_mouse_interaction(game);
    update_player_position(game);
    game->player.rot_angle = normalize_angle(game->player.rot_angle);
    rays_cast(game);
    draw_sprite(game);
    draw_mini_map(game);
    draw_player(game);
    mlx_put_image_to_window(game->mlx, game->win, game->render_buf.img, 0, 0);
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