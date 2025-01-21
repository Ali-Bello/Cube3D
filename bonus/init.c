/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:25:13 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/21 02:36:36 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	init_collectibles(t_bonus_game *game, int id, char *path)
{
	int	i;
	int	n;

	game->world[id].n_coins = rand() % (int)(game->data.map_height * 0.5);
	while (game->world[id].n_coins < 3)
		game->world[id].n_coins = rand() % (int)(game->data.map_height
				* 0.5);
	game->world[id].coins = calloc(game->world[id].n_coins,
			sizeof(t_sprite));
	load_image(&game->data, &game->world[id].coins[0].img, path);
	i = -1;
	if (!id)
		n = 64;
	else
		n = 128;
	while (++i < game->world[id].n_coins)
	{
		generate_valid_coordinates(game, &game->world[id].coins[i].x,
			&game->world[id].coins[i].y);
		if (i > 0)
			game->world[id].coins[i].img = game->world[id].coins[0].img;
		game->world[id].coins[i].frame_width = n;
		game->world[id].coins[i].frame_height = n;
	}
}

void	load_textures(t_bonus_game *game)
{
	load_image(&game->data, &game->world[0].textures[0],
		"./assets/textures/no_wall.xpm");
	load_image(&game->data, &game->world[0].textures[1],
		"./assets/textures/so_wall.xpm");
	load_image(&game->data, &game->world[0].textures[2],
		"./assets/textures/ea_wall.xpm");
	load_image(&game->data, &game->world[0].textures[3],
		"./assets/textures/we_wall.xpm");
	load_image(&game->data, &game->world[0].textures[4],
		"./assets/textures/door.xpm");
	load_image(&game->data, &game->world[1].textures[0],
		"./assets/textures/no_wall2.xpm");
	load_image(&game->data, &game->world[1].textures[1],
		"./assets/textures/so_wall2.xpm");
	load_image(&game->data, &game->world[1].textures[2],
		"./assets/textures/ea_wall2.xpm");
	load_image(&game->data, &game->world[1].textures[3],
		"./assets/textures/we_wall2.xpm");
	load_image(&game->data, &game->world[1].textures[4],
		"./assets/textures/door2.xpm");
	load_image(&game->data, &game->sky, "./assets/textures/sky.xpm");
	load_image(&game->data, &game->portal.img,
		"./assets/textures/portal_sprite.xpm");
	init_collectibles(game, 0, "./assets/textures/coin_sprite.xpm");
	init_collectibles(game, 1, "./assets/textures/coin_sprite2.xpm");
}

void	init_bonus_game(t_bonus_game *game)
{
	srand(time(NULL));
	memset(game, 0, sizeof(t_bonus_game));
	game->data.map = create_map();
	set_height(&game->data);
	game->data.mlx = mlx_init();
	if (!game->data.mlx)
		return ;
	game->data.win = mlx_new_window(game->data.mlx, WIN_WIDTH, WIN_HEIGHT,
			"Cub3D");
	game->data.render_buf.img = mlx_new_image(game->data.mlx, WIN_WIDTH,
			WIN_HEIGHT);
	if (game->data.render_buf.img)
		game->data.render_buf.addr = mlx_get_data_addr(
				game->data.render_buf.img,
				&game->data.render_buf.bpp,
				&game->data.render_buf.line_len,
				&game->data.render_buf.endian);
	game->data.player.x = CUB_SIZE + CUB_SIZE / 2;
	game->data.player.y = CUB_SIZE + CUB_SIZE / 2;
	game->data.player.angle.rad = M_PI / 2;
	game->portal.frame_width = 64;
	game->portal.frame_height = 64;
	load_textures(game);
	game->data.perp_distance = (WIN_WIDTH / 2) / tanf(FOV / 2);
}
