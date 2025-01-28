/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 19:57:27 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/28 22:15:27 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

char	**duplicate_map(char **map, int *height)
{
	char	**new;
	int		i;

	if (*height <= 0 || !map)
		return (NULL);
	new = ft_calloc(*height + 1, sizeof(char *));
	if (!new)
		return (NULL);
	i = -1;
	while (map[++i])
		new[i] = ft_strdup(map[i]);
	i = *height - 1;
	while (i >= 0 && is_empty_line(new[i]))
	{
		free(new[i]);
		free(map[i]);
		map[i] = NULL;
		new[i] = NULL;
		(*height)--;
		i--;
	}
	return (new);
}

char	**extend_2d_array(char **arr, char *new, int *size)
{
	char	**new_arr;
	int		i;

	new_arr = malloc(sizeof(char *) * (*size + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < *size)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i] = ft_strdup(new);
	new_arr[i + 1] = NULL;
	*size += 1;
	free(arr);
	return (new_arr);
}

int	check_valid_map_chars(char *line, char *valid_chars)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_strchr(valid_chars, line[i]) && !ft_isspace(line[i]))
			return (print_err(NULL, "Invalid character in map", 1));
		i++;
	}
	return (0);
}

int	get_map_line(t_parse *parse, char *line)
{
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (check_valid_map_chars(line, parse->valid_set) == 1)
		return (1);
	parse->map = extend_2d_array(parse->map, line, &parse->map_height);
	if (!parse->map)
		return (print_err(NULL, "Could not allocate memory", 1));
	parse->found_map = true;
	return (0);
}
