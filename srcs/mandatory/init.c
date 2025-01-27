/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:25:13 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/27 01:02:44 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

void	get_parsed_data(t_game *game, t_parse *parse)
{
	game->map = parse->map;
	parse->map = NULL;
	game->map_height = parse->map_height;
	game->f_color = parse->floor_color;
	game->c_color = parse->ceil_color;
	game->player.angle.rad += ((M_PI / 2) * (parse->player_dir == 'S'));
	game->player.angle.rad += ((M_PI) * (parse->player_dir == 'W'));
	game->player.angle.rad += ((3 * M_PI / 2) * (parse->player_dir == 'N'));
	game->player.x = (parse->player_x * CUB_SIZE) + CUB_SIZE / 2;
	game->player.y = (parse->player_y * CUB_SIZE) + CUB_SIZE / 2;
}

int	load_image(t_game *data, t_img *img, char *path)
{
	img->img = mlx_xpm_file_to_image(data->mlx, path, &img->width,
			&img->height);
	if (!img->img)
		return (print_err("mlx", "failed to load texture!", 1));
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->line_len,
			&img->endian);
	if (!img->addr)
		return (print_err("mlx", "failed to fetsh texture data", 1));
	return (0);
}

int		load_textures(t_game *game, t_parse *parse)
{
	if (load_image(game, &game->textures[0], parse->no_text))
		return (1);
	if (load_image(game, &game->textures[1], parse->so_text))
		return (1);
	if (load_image(game, &game->textures[2], parse->ea_text))
		return (1);
	if (load_image(game, &game->textures[3], parse->we_text))
		return (1);
	return (0);
}

int	init_game(t_game *game, t_parse *parse)
{
	ft_memset(game, 0, sizeof(t_game));
	get_parsed_data(game, parse);
	game->mlx = mlx_init();
	if (!game->mlx)
		return (print_err("mlx", "failed to init", 1));
	game->render_buf.img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (game->render_buf.img)
		game->render_buf.addr = mlx_get_data_addr(game->render_buf.img,
				&game->render_buf.bpp, &game->render_buf.line_len,
				&game->render_buf.endian);
	if (load_textures(game, parse))
		return (1);
	game->perp_distance = (WIN_WIDTH / 2) / tanf((FOV * M_PI / 180) / 2);
	game->win = mlx_new_window(game->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3D");
	if (!game->win)
		return (print_err("mlx", "failed to create a window", 1));
	return (0);
}

