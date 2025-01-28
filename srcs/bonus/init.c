/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:25:13 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/29 00:06:25 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d_bonus.h"

int	init_collectibles(t_bonus_game *game, int id, char *path)
{
	int	i;
	int	n;

	game->world[id].n_coins = 1 + (rand() % (int)fmax(1,
				(int)((game->data.map_height * 2) * 0.5)));
	game->world[id].coins = calloc(game->world[id].n_coins, sizeof(t_sprite));
	if (load_image(&game->data, &game->world[id].coins[0].img, path))
		return (1);
	i = -1;
	n = 64 * (id == 0) + 128 * (id != 0);
	while (++i < game->world[id].n_coins)
	{
		generate_valid_coordinates(game, &game->world[id].coins[i].x,
			&game->world[id].coins[i].y);
		if (i > 0)
			game->world[id].coins[i].img = game->world[id].coins[0].img;
		game->world[id].coins[i].frame_width = n;
		game->world[id].coins[i].frame_height = n;
	}
	return (0);
}

int	load_bonus_textures(t_bonus_game *game, t_parse *parse)
{
	if (load_image(&game->data, &game->world[0].textures[0], parse->no_text)
		|| load_image(&game->data, &game->world[0].textures[1], parse->so_text)
		|| load_image(&game->data, &game->world[0].textures[2], parse->ea_text)
		|| load_image(&game->data, &game->world[0].textures[3], parse->we_text)
		|| load_image(&game->data, &game->world[0].textures[4],
			"./assets/textures/door.xpm") || load_image(&game->data,
			&game->world[1].textures[0], "./assets/textures/no_wall2.xpm")
		|| load_image(&game->data, &game->world[1].textures[1],
			"./assets/textures/so_wall2.xpm") || load_image(&game->data,
			&game->world[1].textures[2], "./assets/textures/ea_wall2.xpm")
		|| load_image(&game->data, &game->world[1].textures[3],
			"./assets/textures/we_wall2.xpm") || load_image(&game->data,
			&game->world[1].textures[4], "./assets/textures/door2.xpm")
		|| load_image(&game->data, &game->sky, "./assets/textures/sky.xpm")
		|| load_image(&game->data, &game->portal.img,
			"./assets/textures/portal_sprite.xpm"))
		return (1);
	return (0);
}

int	init_buf_img(t_bonus_game *game)
{
	game->data.render_buf.img = mlx_new_image(game->data.mlx, WIN_WIDTH,
			WIN_HEIGHT);
	if (!game->data.render_buf.img)
		return (print_err("mlx", "failed to create new image", 1));
	game->data.render_buf.addr = mlx_get_data_addr(game->data.render_buf.img,
			&game->data.render_buf.bpp, &game->data.render_buf.line_len,
			&game->data.render_buf.endian);
	if (!game->data.render_buf.addr)
		return (print_err("mlx", "failed to get image data", 1));
	return (0);
}

int	init_bonus_game(t_bonus_game *game, t_parse *parse)
{
	srand(time(NULL));
	ft_memset(game, 0, sizeof(t_bonus_game));
	get_parsed_data(&game->data, parse);
	game->data.mlx = mlx_init();
	if (!game->data.mlx)
		return (print_err("mlx", "failed to init", 1));
	if (init_buf_img(game))
		return (1);
	game->portal.frame_width = 64;
	game->portal.frame_height = 64;
	if (load_bonus_textures(game, parse) || init_collectibles(game, 0,
			"./assets/textures/coin_sprite.xpm") || init_collectibles(game, 1,
			"./assets/textures/coin_sprite2.xpm"))
		return (1);
	game->data.perp_distance = (WIN_WIDTH / 2) / tanf((FOV * M_PI / 180) / 2);
	game->data.win = mlx_new_window(game->data.mlx, WIN_WIDTH, WIN_HEIGHT,
			"cub3D");
	if (!game->data.win)
		return (print_err("mlx", "failed to create a window", 1));
	return (0);
}
