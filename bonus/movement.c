/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 02:33:20 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/21 21:55:14 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

bool	check_surrondings(char **map, float x, float y, char c)
{
	if (map[(int)((y - 10.0) / CUB_SIZE)][(int)((x) / CUB_SIZE)] == c
		|| map[(int)((y + 10.0) / CUB_SIZE)][(int)((x) / CUB_SIZE)] == c
		|| map[(int)((y) / CUB_SIZE)][(int)((x - 10.0) / CUB_SIZE)] == c
		|| map[(int)((y) / CUB_SIZE)][(int)((x + 10.0) / CUB_SIZE)] == c)
		return (true);
	return (false);
}

bool	wall_collision_check_bonus(t_bonus_game *game, float x, float y)
{
	int	map_x;
	int	map_y;

	map_x = (int)(floorf(x) / CUB_SIZE);
	map_y = (int)(floorf(y) / CUB_SIZE);
	if (game->data.map[map_y][map_x] == '1' || map_x < 0 || map_y < 0)
		return (true);
	if (!game->door_open && check_surrondings(game->data.map, x, y, 'D'))
		return (true);
	return (check_surrondings(game->data.map, x, y, '1'));
}

void	update_mouse_interaction(t_bonus_game *game)
{
	int	x;
	int	y;
	int	dx;

	mlx_mouse_get_pos(game->data.mlx, game->data.win, &x, &y);
	dx = (x - WIN_WIDTH / 2);
	if (dx != 0)
	{
		game->data.player.angle.rad += (ROT_SPEED * M_PI / 180) * dx / 35.0;
		mlx_mouse_move(game->data.mlx, game->data.win, WIN_WIDTH / 2, WIN_HEIGHT
			/ 2);
	}
}

void	move_player(t_bonus_game *game, float x, float y)
{
	if (!wall_collision_check_bonus(game, x, y))
	{
		game->data.player.x = x;
		game->data.player.y = y;
	}
	else
	{
		if (!wall_collision_check_bonus(game, x,
				game->data.player.y))
			game->data.player.x = x;
		else if (!wall_collision_check_bonus(game, game->data.player.x,
				y))
			game->data.player.y = y;
	}
}

void	update_player_position(t_bonus_game *game)
{
	float	move_step;
	float	strafe_step;
	float	next_map_player_x;
	float	next_map_player_y;

	move_step = game->data.player.walk_dir * WALK_SPEED;
	strafe_step = game->data.player.strafe_dir * WALK_SPEED;
	next_map_player_x = game->data.player.x + (game->data.player.angle.cos
			* move_step) + (cosf(game->data.player.angle.rad + M_PI_2)
			* strafe_step);
	next_map_player_y = game->data.player.y + (game->data.player.angle.sin
			* move_step) + (sinf(game->data.player.angle.rad + M_PI_2)
			* strafe_step);
	game->data.player.angle.rad += (game->data.player.turn_dir
			* (ROT_SPEED * M_PI / 180));
	move_player(game, next_map_player_x, next_map_player_y);
}
