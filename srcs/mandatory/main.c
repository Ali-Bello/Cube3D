/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:25:57 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/28 23:14:33 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

void	rays_cast(t_game *game)
{
	int		i;
	float	angle;
	t_ray	ray;

	i = 0;
	angle = game->player.angle.rad - ((FOV * (M_PI / 180)) / 2.0);
	while (i < WIN_WIDTH)
	{
		angle = game->player.angle.rad + (atan2f((i - WIN_WIDTH / 2.0),
					game->perp_distance));
		cast_ray(game, &ray, angle);
		draw_ray(game, &ray, i);
		i++;
	}
}

bool	wall_collision_check(t_game *game, float x, float y)
{
	int	map_x;
	int	map_y;

	map_x = (int)(x / CUB_SIZE);
	map_y = (int)(y / CUB_SIZE);
	if (game->map[map_y][map_x] == '1' || map_x < 0 || map_y < 0)
		return (true);
	if (game->map[(int)((y - 10.0) / CUB_SIZE)][(int)((x) / CUB_SIZE)] == '1'
		|| game->map[(int)((y + 10.0) / CUB_SIZE)][(int)((x) / CUB_SIZE)] == '1'
		|| game->map[(int)((y) / CUB_SIZE)][(int)((x - 10.0) / CUB_SIZE)] == '1'
		|| game->map[(int)((y) / CUB_SIZE)][(int)((x + 10.0)
			/ CUB_SIZE)] == '1')
		return (true);
	return (false);
}

void	update_player_position(t_game *game)
{
	float	move_step;
	float	strafe_step;
	float	next_map_player_x;
	float	next_map_player_y;

	move_step = game->player.walk_dir * WALK_SPEED;
	strafe_step = game->player.strafe_dir * WALK_SPEED;
	next_map_player_x = game->player.x + (game->player.angle.cos * move_step)
		+ (cosf(game->player.angle.rad + M_PI_2) * strafe_step);
	next_map_player_y = game->player.y + (game->player.angle.sin * move_step)
		+ (sinf(game->player.angle.rad + M_PI_2) * strafe_step);
	game->player.angle.rad += (game->player.turn_dir * (ROT_SPEED * (M_PI
					/ 180)));
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

int	update(t_game *game)
{
	game->player.angle.cos = cosf(game->player.angle.rad);
	game->player.angle.sin = sinf(game->player.angle.rad);
	game->player.angle.tan = tanf(game->player.angle.rad);
	game->player.angle.rad = normalize_angle(game->player.angle.rad);
	update_player_position(game);
	rays_cast(game);
	mlx_put_image_to_window(game->mlx, game->win, game->render_buf.img, 0, 0);
	return (0);
}

int	main(int ac, char **av)
{
	t_game	game;
	t_parse	parse;

	if (ac != 2)
		return (print_err("Wrong input",
				"\nUsage: [./cub3D] [path_to_cub_file]", 1), EXIT_FAILURE);
	if (check_constants())
		return (print_err(NULL, "Invalid Constants", 1), EXIT_FAILURE);
	ft_memset(&parse, 0, sizeof(t_parse));
	parse.floor_color = -1;
	parse.ceil_color = -1;
	parse.valid_set = "01NEWS";
	if (ft_parse(&parse, av[1]))
		return (free_parser(&parse), EXIT_FAILURE);
	if (init_game(&game, &parse))
		return (free_parser(&parse), exit_routine(&game));
	free_parser(&parse);
	mlx_hook(game.win, 17, 0, exit_routine, &game);
	mlx_hook(game.win, 2, (1L << 0), key_press, &game);
	mlx_hook(game.win, 3, (1L << 1), key_release, &game);
	mlx_loop_hook(game.mlx, &update, &game);
	mlx_loop(game.mlx);
	return (0);
}
