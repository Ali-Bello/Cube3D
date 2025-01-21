/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:39:41 by marvin            #+#    #+#             */
/*   Updated: 2025/01/19 23:36:19 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int exit_routine(t_game *game)
{
    int i;

    system("pkill vlc");
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

int key_press(int key, t_game *game)
{
    if (key == XK_Escape)
        exit_routine(game);
    if (key == XK_w)
        game->player.walk_dir = 1;
    else if (key == XK_a)
        game->player.strafe_dir = -1;
    else if (key == XK_s)
        game->player.walk_dir = -1;
    else if (key == XK_d)
        game->player.strafe_dir = 1;
    else if (key == XK_Left)
        game->player.turn_dir = -1;
    else if (key == XK_Right)
        game->player.turn_dir = 1;
    else if (key == XK_space && game->world[game->world_idx].collected_coins == game->world[game->world_idx].n_coins)
        game->spawn_portal= true;
    else if (key == XK_Control_L)
    {
        game->mouse_mode = !game->mouse_mode;
        if (!game->mouse_mode)
            mlx_mouse_hide(game->mlx, game->win);
        else
            mlx_mouse_show(game->mlx, game->win);
    }
    else if (key == XK_e && game->door_inrange)
    {
        if (game->door_open)
            play_sound("./assets/sfx/door_close.mp3", "0.2");
        else
            play_sound("./assets/sfx/door_open.mp3", "0.2");
        game->door_open = !game->door_open;
    }
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
