/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:40:23 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/27 00:38:54 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

static bool	is_dir(char *arg)
{
	int		fd;
	bool	ret;

	ret = false;
	fd = open(arg, O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		ret = true;
	}
	return (ret);
}

bool	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line && line[i])
	{
		if (line[i] != ' ' && !(line[i] >= 9 && line[i] <= 13))
			return (false);
		i++;
	}
	return (true);
}

static int	handle_pre_map_content(t_parse *parse, char *line)
{
	if (!ft_strncmp(line, "NO ", 3) || !ft_strncmp(line, "SO ", 3)
		|| !ft_strncmp(line, "WE ", 3) || !ft_strncmp(line, "EA ", 3))
		return (parse_textures(parse, line));
	else if (!ft_strncmp(line, "F ", 2) || !ft_strncmp(line, "C ", 2))
		return (parse_colors(parse, line));
	else if (parse->floor_color != -1 && parse->ceil_color != -1
		&& ft_strchr("01NSWE \t\r", *line))
		return (get_map_line(parse, line));
	else
		return (print_err(NULL, "Invalid configuration", 1), 1);
	return (0);
}

int	handle_line(t_parse *parse, char *line)
{
	bool	line_empty;

	line_empty = is_empty_line(line);
	if (!parse->found_map && line_empty == false)
	{
		if (handle_pre_map_content(parse, line) == 1)
			return (close(parse->fd), free(line), 1);
	}
	else if (parse->found_map)
	{
		if (get_map_line(parse, line) == 1)
			return (close(parse->fd), free(line), 1);
	}
	return (0);
}

int	ft_parse(t_parse *parse, char *path)
{
	char	*line;

	if (path && ft_strncmp(path + ft_strlen(path) - 4, ".cub", 4))
		return (print_err(NULL, "Invalid map file", 1), 1);
	parse->fd = open(path, O_RDONLY);
	if (parse->fd < 0 || is_dir(path))
		return (print_err(path, "map isn't a regular file", 1));
	line = get_next_line(parse->fd, 0);
	while (line)
	{
		if (handle_line(parse, line) == 1)
			return (get_next_line(parse->fd, true), 1);
		(void)(free(line), line = get_next_line(parse->fd, 0));
	}
	if (validate_and_close(parse) == 1)
		return (1);
	return (0);
}
