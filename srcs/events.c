/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:39:41 by marvin            #+#    #+#             */
/*   Updated: 2024/12/15 16:39:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int exit_routine(t_game *game)
{
    int i;

    i = 0;
    while (game->map[i])
    {
        free(game->map[i]);
        i++;
    }
    free(game->map);
    mlx_destroy_image(game->mlx, game->render_buf.img);
    mlx_destroy_window(game->mlx, game->win);
    exit(0);
}
int key_press(int key, t_game *game)
{
    if (key == ESC)
        exit_routine(game);
    if (key == W)
        game->player.walk_dir = 1;
    else if (key == A)
        game->player.turn_dir = -1;
    else if (key == S)
        game->player.walk_dir = -1;
    else if (key == D)
        game->player.turn_dir = 1;
    return (0);
}

int key_release(int key, t_game *game)
{
    if (key == W || key == S)
        game->player.walk_dir = 0;
    else if (key == A || key == D)
        game->player.turn_dir = 0;
    return (0);
}
