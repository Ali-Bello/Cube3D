/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:39:41 by marvin            #+#    #+#             */
/*   Updated: 2025/01/21 02:38:27 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	exit_routine_bonus(t_bonus_game *game)
{
	int	i;

	system("pkill vlc");
	i = 0;
	while (game->data.map && game->data.map[i])
	{
		free(game->data.map[i]);
		i++;
	}
	free(game->data.map);
	if (game->data.mlx)
	{
		if (game->data.render_buf.img)
			mlx_destroy_image(game->data.mlx, game->data.render_buf.img);
		if (game->data.win)
			mlx_destroy_window(game->data.mlx, game->data.win);
		mlx_destroy_display(game->data.mlx);
	}
	exit(0);
}

void	update_extras(int key, t_bonus_game *game)
{
	if (key == XK_space
		&& game->world[game->world_idx].collected_coins
		== game->world[game->world_idx].n_coins)
		game->spawn_portal = true;
	else if (key == XK_Control_L)
	{
		game->mouse_mode = !game->mouse_mode;
		if (!game->mouse_mode)
			mlx_mouse_hide(game->data.mlx, game->data.win);
		else
			mlx_mouse_show(game->data.mlx, game->data.win);
	}
	else if (key == XK_e && game->door_inrange)
	{
		if (!check_surrondings(game->data.map, game->data.player.x,
				game->data.player.y, 'D'))
		{
			if (game->door_open)
				play_sound("./assets/sfx/door_close.mp3", "0.2");
			else
				play_sound("./assets/sfx/door_open.mp3", "0.2");
			game->door_open = !game->door_open;
		}
	}
}

int	key_press_bonus(int key, t_bonus_game *game)
{
	if (key == XK_Escape)
		exit_routine_bonus(game);
	if (key == XK_w)
		game->data.player.walk_dir = 1;
	else if (key == XK_a)
		game->data.player.strafe_dir = -1;
	else if (key == XK_s)
		game->data.player.walk_dir = -1;
	else if (key == XK_d)
		game->data.player.strafe_dir = 1;
	else if (key == XK_Left)
		game->data.player.turn_dir = -1;
	else if (key == XK_Right)
		game->data.player.turn_dir = 1;
	else
		update_extras(key, game);
	return (0);
}

int	key_release_bonus(int key, t_bonus_game *game)
{
	if (key == XK_w || key == XK_s)
		game->data.player.walk_dir = 0;
	else if (key == XK_a || key == XK_d)
		game->data.player.strafe_dir = 0;
	else if (key == XK_Left || key == XK_Right)
		game->data.player.turn_dir = 0;
	return (0);
}
