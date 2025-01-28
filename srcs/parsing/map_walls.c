/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_walls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:52:39 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/28 22:20:40 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

bool	check_empty_gaps(t_parse *parse, char **map, int i)
{
	size_t	j;

	j = 0;
	while (map[i][j])
	{
		if (map[i][j] == '0')
		{
			if (!map[i][j + 1] || !ft_strchr(parse->valid_set, map[i][j + 1]))
				return (false);
			if (i <= 0 || j >= ft_strlen(map[i - 1])
				|| !ft_strchr(parse->valid_set, map[i - 1][j]))
				return (false);
			if (i >= parse->map_height || j >= ft_strlen(map[i + 1])
				|| !ft_strchr(parse->valid_set, map[i + 1][j]))
				return (false);
		}
		j++;
	}
	return (true);
}

bool	check_boundaries(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (!line[i] || line[i] != '1')
		return (false);
	i = ft_strlen(line) - 1;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (i < 0 || line[i] != '1')
		return (false);
	return (true);
}

bool	check_player_surronding(t_parse *parse, int i, int j)
{
	if (j <= 0 || ft_isspace(parse->map[i][j - 1]))
		return (true);
	if (j >= (int)ft_strlen(parse->map[i]) || ft_isspace(parse->map[i][j + 1]))
		return (true);
	if (i <= 0 || j >= (int)ft_strlen(parse->map[i - 1])
		|| ft_isspace(parse->map[i - 1][j]))
		return (true);
	if (i >= parse->map_height || j >= (int)ft_strlen(parse->map[i + 1])
		|| ft_isspace(parse->map[i + 1][j]))
		return (true);
	parse->player_x = j;
	parse->player_y = i;
	parse->player_dir = parse->map[i][j];
	return (false);
}

bool	check_player(t_parse *parse)
{
	int	i;
	int	j;
	int	count;

	i = -1;
	count = 0;
	while (parse->map[++i])
	{
		j = -1;
		while (parse->map[i][++j])
		{
			if (ft_strchr("NEWS", parse->map[i][j]))
			{
				if (check_player_surronding(parse, i, j))
					return (false);
				count++;
			}
		}
	}
	if (count != 1)
		return (false);
	return (true);
}
