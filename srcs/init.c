/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:25:13 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/16 11:28:16 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char    **create_map()
{
    char    **map;

    map = calloc(14, sizeof(char *));
    map[0] = strdup("11111111111111111111111111111");
    map[1] = strdup("1N000000000000000000000000001");
    map[2] = strdup("10001110000000000000000000001");
    map[3] = strdup("10000000000000000000000000001");
    map[4] = strdup("11100001000001000000000000001");
    map[5] = strdup("10100001000000100000000000001");
    map[6] = strdup("10001000000000000000000000111");
    map[7] = strdup("10001100000000000000000000001");
    map[8] = strdup("10001000000000000000000000001");
    map[9] = strdup("10001000000000000000000000001");
    map[10] = strdup("1000100000011D1D0000000000001");
    map[11] = strdup("10001000000100100000000000001");
    map[12] = strdup("11111111111111111111111111111");
    map[13] = NULL;
    return (map);
}

void    set_width_height(t_game *data)
{
    int     i;

    if (!data->map)
        return ;
    i = 0;
    while (data->map[i])
        i++;
    data->map_height = i;
    data->map_width = strlen(data->map[0]);
}

void    load_image(t_game *data, t_img *img, char *path)
{
    img->img = mlx_xpm_file_to_image(data->mlx, path, &img->width, &img->height);
    if (!img->img)
    {
        write(2, "ERROR: failed to load texture!\n", 31);
        exit_routine(data);
    }
    img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->line_len, &img->endian);
    if (!img->addr)
    {
        write(2, "ERROR: mlx failed to fetsh texture data!\n", 41);
        exit_routine(data);
    }
}

void    init_collectibles(t_game *game)
{
    int i;

    game->collectibles_count = rand() % ((game->map_width * game->map_height) * 3 / 100);
    game->collectibles = calloc(game->collectibles_count, sizeof(t_sprite));
    load_image(game, &game->collectibles[0].img, "./textures/coin_sprite.xpm");
    i = -1;
    while (++i < game->collectibles_count)
    {
        generate_valid_coordinates(game, &game->collectibles[i].x, &game->collectibles[i].y);
        if (i > 0)
            game->collectibles[i].img = game->collectibles[0].img;
        game->collectibles[i].frame_width = 64;
        game->collectibles[i].frame_height = 64;
    }
}
void    init_game(t_game *data)
{
    memset(data, 0, sizeof(t_game));
    data->map = create_map();
    set_width_height(data);
    data->mlx = mlx_init();
    if (!data->mlx)
        return ;
    data->win = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, "Cube3D");
    data->render_buf.img = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT);
    if (data->render_buf.img)
        data->render_buf.addr = mlx_get_data_addr(data->render_buf.img, &data->render_buf.bpp,
                        &data->render_buf.line_len, &data->render_buf.endian);
    data->player.x = CUB_SIZE + 16;
    data->player.y = CUB_SIZE + 16;
    data->player.angle.rad = M_PI / 2;
    load_image(data, &data->textures[0], "./textures/no_wall.xpm");
    load_image(data, &data->textures[1], "./textures/so_wall.xpm");
    load_image(data, &data->textures[2], "./textures/ea_wall.xpm");
    load_image(data, &data->textures[3], "./textures/we_wall.xpm");
    load_image(data, &data->textures[4], "./textures/door.xpm");
    load_image(data, &data->textures[5], "./textures/sky.xpm");
    /* portal */
    load_image(data, &data->portal.img, "./textures/portal_sprite.xpm");
    data->portal.frame_width = 64;
    data->portal.frame_height = 64;
    /**********/
    init_collectibles(data);
    data->last_mouse_x = -1;
    data->perp_distance = (WIN_WIDTH / 2) / tanf(FOV / 2);
}

void    check_allocations(t_game *data)
{
    if (!data->mlx || !data->win)
    {
        perror("ERROR: mlx failed!\n");
        exit_routine(data);
    }
    if (!data->map)
    {
        perror("ERROR: malloc failed!\n");
        exit_routine(data);
    }
}
