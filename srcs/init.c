/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:25:13 by aderraj           #+#    #+#             */
/*   Updated: 2024/12/21 14:02:06 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char    **create_map()
{
    char    **map;

    map = calloc(14, sizeof(char *));
    map[0] = strdup("11111111111111111111111111111");
    map[1] = strdup("10000000000000000000000000001");
    map[2] = strdup("10001110000000000000000000001");
    map[3] = strdup("10000000000000000000000000001");
    map[4] = strdup("10100001000001000000000000001");
    map[5] = strdup("10100001000000100000000000001");
    map[6] = strdup("10001000000000000000000000111");
    map[7] = strdup("10001100000000000000000000001");
    map[8] = strdup("10001000000000000000000000001");
    map[9] = strdup("10000000000000000000000000001");
    map[10] = strdup("10000000000000001000000000001");
    map[11] = strdup("10000000000000001000000000001");
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
    data->player.x = CUB_SIZE + 32;
    data->player.y = CUB_SIZE + 32;
    data->player.rot_angle = M_PI / 2;
    data->textures.img = mlx_xpm_file_to_image(data->mlx, "wall.xpm",\
                        &data->textures.width, &data->textures.height);
    data->textures.addr = mlx_get_data_addr(data->textures.img, &data->textures.bpp,\
                        &data->textures.line_len, &data->textures.endian);
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
