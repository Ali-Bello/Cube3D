/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 01:29:47 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/27 00:38:54 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

bool	check_enclosure(char **map, int height)
{
	int	i;

	i = -1;
	if (!map)
		return (false);
	while (map[++i + 1])
	{
		if (!check_boundaries(map[i]))
			return (false);
		if (!check_empty_gaps(map, i, height))
			return (false);
	}
	return (true);
}

bool	check_top_and_bottom(char **map, int rows)
{
	int	i;

	i = 0;
	while (map[0][i])
	{
		if (map[0][i] != '1' && !ft_isspace(map[0][i]))
			return (false);
		i++;
	}
	i = 0;
	while (map[rows - 1][i])
	{
		if (map[rows - 1][i] != '1' && !ft_isspace(map[rows - 1][i]))
			return (false);
		i++;
	}
	return (true);
}

bool	validate_map(t_parse *parse)
{
	char	**copy;

	if (parse->map_height < 3)
		return (false);
	copy = duplicate_map(parse->map, &parse->map_height);
	if (!copy)
		return (false);
	if (!check_enclosure(copy, parse->map_height))
		return (free_2d_array(copy), false);
	if (!check_top_and_bottom(copy, parse->map_height))
		return (free_2d_array(copy), false);
	if (!check_player(parse))
		return (free_2d_array(copy), false);
	return (free_2d_array(copy), true);
}

int	validate_and_close(t_parse *parse)
{
	if (!validate_map(parse))
	{
		close(parse->fd);
		return (print_err(NULL, "Invalid map", 1));
	}
	if (parse->texture_count != 4 || parse->floor_color == -1
		|| parse->ceil_color == -1)
	{
		close(parse->fd);
		return (print_err(NULL, "Missing required configurations", 1));
	}
	return (close(parse->fd), 0);
}
