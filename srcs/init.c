/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:25:13 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/16 06:16:54 by aderraj          ###   ########.fr       */
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
    data->textures[0].img = mlx_xpm_file_to_image(data->mlx, "./textures/no_wall.xpm",\
                        &data->textures[0].width, &data->textures[0].height);
    data->textures[0].addr = mlx_get_data_addr(data->textures[0].img, &data->textures[0].bpp,\
                        &data->textures[0].line_len, &data->textures[0].endian);
    data->textures[1].img = mlx_xpm_file_to_image(data->mlx, "./textures/so_wall.xpm",\
                        &data->textures[1].width, &data->textures[1].height);
    data->textures[1].addr = mlx_get_data_addr(data->textures[1].img, &data->textures[1].bpp,\
                        &data->textures[1].line_len, &data->textures[1].endian);
    data->textures[2].img = mlx_xpm_file_to_image(data->mlx, "./textures/ea_wall.xpm",\
                        &data->textures[2].width, &data->textures[2].height);
    data->textures[2].addr = mlx_get_data_addr(data->textures[2].img, &data->textures[2].bpp,\
                        &data->textures[2].line_len, &data->textures[2].endian);
    data->textures[3].img = mlx_xpm_file_to_image(data->mlx, "./textures/we_wall.xpm",\
                        &data->textures[3].width, &data->textures[3].height);
    data->textures[3].addr = mlx_get_data_addr(data->textures[3].img, &data->textures[3].bpp,\
                        &data->textures[3].line_len, &data->textures[3].endian);
    data->textures[4].img = mlx_xpm_file_to_image(data->mlx, "./textures/door.xpm",\
                        &data->textures[4].width, &data->textures[4].height);
    data->textures[4].addr = mlx_get_data_addr(data->textures[4].img, &data->textures[4].bpp,\
                        &data->textures[4].line_len, &data->textures[4].endian);
    data->textures[5].img = mlx_xpm_file_to_image(data->mlx, "./textures/sky.xpm",\
                        &data->textures[5].width, &data->textures[5].height);
    data->textures[5].addr = mlx_get_data_addr(data->textures[5].img, &data->textures[5].bpp,\
                        &data->textures[5].line_len, &data->textures[5].endian);
    /* portal */
    data->portal.img.img = mlx_xpm_file_to_image(data->mlx, "./textures/portal_sprite.xpm",\
                        &data->portal.img.width, &data->portal.img.height);
    data->portal.img.addr = mlx_get_data_addr(data->portal.img.img, &data->portal.img.bpp,\
                        &data->portal.img.line_len, &data->portal.img.endian);
    data->portal.frame_width = 64;
    data->portal.frame_height = 64;
        // data->sprite.offset_x = 64;
    /**********/
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
