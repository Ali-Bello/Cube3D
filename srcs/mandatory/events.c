/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:39:41 by marvin            #+#    #+#             */
/*   Updated: 2025/01/27 01:06:01 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

int	exit_routine(t_game *game)
{
	int	i;

	i = -1;
	while (game->map && game->map[++i])
		free(game->map[i]);
	free(game->map);
	if (game->mlx)
	{
		i = -1;
		while (++i < 4)
		{
			if (game->textures[i].img)
				mlx_destroy_image(game->mlx, game->textures[i].img);
		}
		if (game->render_buf.img)
			mlx_destroy_image(game->mlx, game->render_buf.img);
		if (game->win)
			mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	exit(0);
}

int	key_press(int key, t_game *game)
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
	return (0);
}

int	key_release(int key, t_game *game)
{
	if (key == XK_w || key == XK_s)
		game->player.walk_dir = 0;
	else if (key == XK_a || key == XK_d)
		game->player.strafe_dir = 0;
	else if (key == XK_Left || key == XK_Right)
		game->player.turn_dir = 0;
	return (0);
}
