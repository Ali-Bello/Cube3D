/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 01:58:11 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/29 00:30:47 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d_bonus.h"

void	rays_cast(t_bonus_game *game)
{
	int		i;
	float	angle;
	t_ray	ray;

	i = 0;
	while (i < WIN_WIDTH)
	{
		angle = game->data.player.angle.rad + (atan2f((i - WIN_WIDTH / 2.0),
					game->data.perp_distance));
		cast_ray_bonus(game, &ray, angle);
		game->distances[i] = ray.casted.distance;
		draw_ray_bonus(game, &ray, i);
		draw_mini_ray(game, &ray);
		if (ray.casted.distance > 9.0 && ray.casted.distance < 49.0
			&& game->data.map[(int)(ray.wall_hit.y
				/ CUB_SIZE)][(int)(ray.wall_hit.x / CUB_SIZE)] == 'D')
			game->door_inrange = true;
		else
			game->door_inrange = false;
		i++;
	}
}

int	update(t_bonus_game *game)
{
	game->data.player.angle.cos = cosf(game->data.player.angle.rad);
	game->data.player.angle.sin = sinf(game->data.player.angle.rad);
	game->data.player.angle.tan = tanf(game->data.player.angle.rad);
	game->data.player.angle.rad = normalize_angle(game->data.player.angle.rad);
	clear_mini_map_area(&game->data.render_buf);
	if (!game->mouse_mode)
		update_mouse_interaction(game);
	update_player_position(game);
	mlx_clear_window(game->data.mlx, game->data.win);
	rays_cast(game);
	if (game->world[game->world_idx].collected_coins
		!= game->world[game->world_idx].n_coins)
		update_collectibles(game);
	if (game->spawn_portal)
		update_portal(game);
	draw_mini_map(game);
	mlx_put_image_to_window(game->data.mlx, game->data.win,
		game->data.render_buf.img, 0, 0);
	return (0);
}

void	*call_sound_cmd(void *result)
{
	system((char *)result);
	free(result);
	return (NULL);
}

void	play_sound(t_bonus_game *game, char *path, char *gain)
{
	char		*result;
	pthread_t	thread;

	(void)game;
	result = calloc(100, sizeof(char));
	if (result == NULL)
		return ;
	snprintf(result, 100,
		"cvlc --gain=%s --play-and-exit --quiet %s\
		> /dev/null 2>&1", gain, path);
	pthread_create(&thread, NULL, call_sound_cmd, result);
	pthread_detach(thread);
}

int	main(int ac, char **av)
{
	t_bonus_game	game;
	t_parse			parse;

	if (ac != 2)
		return (print_err("Wrong input\n",
				"Usage: [./cub3D] [path_to_cub_file]", 1), EXIT_FAILURE);
	if (check_constants())
		return (print_err(NULL, "Invalid Constants", 1), EXIT_FAILURE);
	ft_memset(&parse, 0, sizeof(t_parse));
	parse.floor_color = -1;
	parse.ceil_color = -1;
	parse.valid_set = "01NEWSD";
	if (ft_parse(&parse, av[1]))
		return (free_parser(&parse), EXIT_FAILURE);
	if (init_bonus_game(&game, &parse))
		return (free_parser(&parse), exit_routine_bonus(&game));
	free_parser(&parse);
	mlx_mouse_hide(game.data.mlx, game.data.win);
	mlx_hook(game.data.win, 17, 0, exit_routine_bonus, &game);
	mlx_hook(game.data.win, 2, (1L << 0), key_press_bonus, &game);
	mlx_hook(game.data.win, 3, (1L << 1), key_release_bonus, &game);
	mlx_loop_hook(game.data.mlx, &update, &game);
	play_sound(&game, "./assets/sfx/background.mp3", "0.09");
	mlx_loop(game.data.mlx);
	return (0);
}
