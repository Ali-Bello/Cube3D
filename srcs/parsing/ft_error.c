/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:31:21 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/28 23:15:41 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

int	check_constants(void)
{
	return (FOV <= 0 || FOV > 130 || CUB_SIZE <= 0 || WALK_SPEED <= 0
		|| ROT_SPEED <= 0 || WIN_WIDTH <= 10 || WIN_HEIGHT <= 10
		|| WIN_WIDTH > 1920 || WIN_HEIGHT > 1080
		|| MINI_MAP_SCALE_FACTOR >= 1.2);
}

int	print_err(char *info, char *str, int code)
{
	ft_putstr_fd("cub3D: Error", 2);
	if (info)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(info, 2);
	}
	if (str)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(str, 2);
	}
	ft_putstr_fd("\n", 2);
	return (code);
}

void	free_2d_array(char **str)
{
	int	i;

	i = -1;
	while (str && str[++i])
		free(str[i]);
	if (str)
	{
		free(str);
		str = NULL;
	}
}

void	free_parser(t_parse *parse)
{
	if (parse->map != NULL)
		free_2d_array(parse->map);
	if (parse->no_text != NULL)
		free(parse->no_text);
	if (parse->so_text != NULL)
		free(parse->so_text);
	if (parse->we_text != NULL)
		free(parse->we_text);
	if (parse->ea_text != NULL)
		free(parse->ea_text);
}
