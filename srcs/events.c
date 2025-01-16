/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:39:41 by marvin            #+#    #+#             */
/*   Updated: 2025/01/16 06:26:42 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int exit_routine(t_game *game)
{
    int i;

    i = 0;
    while (game->map && game->map[i])
    {
        free(game->map[i]);
        i++;
    }
    free(game->map);
    if (game->mlx)
    {
        if (game->render_buf.img)
            mlx_destroy_image(game->mlx, game->render_buf.img);
        if (game->win)
            mlx_destroy_window(game->mlx, game->win);
        mlx_destroy_display(game->mlx);
    }
    exit(0);
}

bool    check_door_surrondings(char **map, float x, float y, char c)
{
    if (map[(int)((y - 11.0) / CUB_SIZE)][(int)((x) / CUB_SIZE)] == c ||
        map[(int)((y + 11.0) / CUB_SIZE)][(int)((x) / CUB_SIZE)] == c ||
        map[(int)((y) / CUB_SIZE)][(int)((x - 11.0) / CUB_SIZE)] == c ||
        map[(int)((y) / CUB_SIZE)][(int)((x + 11.0) / CUB_SIZE)] == c)
        return (true);
    return (false);
}

int key_press(int key, t_game *game)
{
    bool    door_surrondings;

    if (key == ESC)
        exit_routine(game);
    if (key == W)
        game->player.walk_dir = 1;
    else if (key == A)
        game->player.strafe_dir = -1;
    else if (key == S)
        game->player.walk_dir = -1;
    else if (key == D)
        game->player.strafe_dir = 1;
    else if (key == LEFT)
        game->player.turn_dir = -1;
    else if (key == RIGHT)
        game->player.turn_dir = 1;
    else if (key == SPACE)
        game->spawn_portal= true;
    else if (key == E)
    {
        door_surrondings = check_door_surrondings(game->map, game->player.x, game->player.y, 'D');       
        if (game->door_open && !door_surrondings)
            game->door_open = false;
        else if (!game->door_open && door_surrondings)
            game->door_open = true;
    }
    // game->player.angle.rad += (game->player.turn_dir * ROT_SPEED);
    return (0);
}

int key_release(int key, t_game *game)
{
    if (key == W || key == S)
        game->player.walk_dir = 0;
    else if (key == A || key == D)
        game->player.strafe_dir = 0;
    else if (key == LEFT || key == RIGHT)
        game->player.turn_dir = 0;
    return (0);
}
