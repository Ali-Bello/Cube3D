/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:31:30 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/27 00:38:54 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

char	*get_texture_path(char *path)
{
	int		len;
	int		i;
	char	*line;

	len = 0;
	while (path[len])
		len++;
	if (len < 5)
		return (print_err(NULL, "Invalid texture path", 1), NULL);
	line = malloc(sizeof(char) * (&path[len] - path + 1));
	if (!line)
		return (NULL);
	i = -1;
	while (path[++i])
		line[i] = path[i];
	line[i] = '\0';
	return (line);
}

int	fill_texures_paths(t_parse *parse, char *path, char c)
{
	char	*texture_path;

	texture_path = get_texture_path(path);
	if (!texture_path)
		return (1);
	if (c == 'N' && !(parse->no_text))
		parse->no_text = texture_path;
	else if (c == 'S' && !(parse->so_text))
		parse->so_text = texture_path;
	else if (c == 'W' && !(parse->we_text))
		parse->we_text = texture_path;
	else if (c == 'E' && !(parse->ea_text))
		parse->ea_text = texture_path;
	else
		return (free(texture_path), 1);
	return (0);
}

int	parse_textures(t_parse *parse, char *line)
{
	char	*stock;

	stock = ft_strtrim(line + 3, " \t\r\n");
	if (ft_strncmp(stock + ft_strlen(stock) - 4, ".xpm", 4))
		return (print_err(stock, "NOT A [.xpm] file", 1), free(stock), 1);
	if (fill_texures_paths(parse, stock, line[0]) == 1)
		return (free(stock), 1);
	free(stock);
	parse->texture_count++;
	return (0);
}
