/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 19:55:12 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/28 21:58:07 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/cub3d.h"

static int	rgb_to_int(char **rgb_to_convert)
{
	int	r;
	int	g;
	int	b;

	r = ft_atoi(rgb_to_convert[0]);
	g = ft_atoi(rgb_to_convert[1]);
	b = ft_atoi(rgb_to_convert[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return (((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
}

bool	check_valid_num(char *str)
{
	int	i;

	i = 0;
	if (!str || ft_strlen(str) > 3)
		return (false);
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (false);
		i++;
	}
	return (true);
}

int	set_rgb_colors(char *line)
{
	char	**rgb_to_convert;
	int		count;
	int		color;

	rgb_to_convert = ft_split(line, ',');
	if (!rgb_to_convert)
		return (print_err(NULL, "Could not allocate Memory", 0), -1);
	count = 0;
	while (rgb_to_convert[count])
	{
		if (!check_valid_num(rgb_to_convert[count]))
			break ;
		else
			count++;
	}
	if (count != 3)
	{
		free_2d_array(rgb_to_convert);
		return (-1);
	}
	color = rgb_to_int(rgb_to_convert);
	free_2d_array(rgb_to_convert);
	return (color);
}

int	fill_color_values(t_parse *parse, char *line, char c)
{
	if (parse->floor_color == -1 && c == 'F')
	{
		parse->floor_color = set_rgb_colors(line);
		if (parse->floor_color == -1)
			return (print_err(NULL, "Invalid floor color", 1));
	}
	else if (parse->ceil_color == -1 && c == 'C')
	{
		parse->ceil_color = set_rgb_colors(line);
		if (parse->ceil_color == -1)
			return (print_err(NULL, "Invalid ceiling color", 1));
	}
	else
		return (print_err(NULL, "Invalid Color", 1));
	return (0);
}

int	parse_colors(t_parse *parse, char *line)
{
	char	*stock;
	int		comma_count;
	int		i;

	comma_count = 0;
	i = -1;
	stock = ft_strtrim(line + 1, " \t\r\n");
	while (stock && stock[++i])
	{
		if (stock[i] == ',')
			comma_count++;
	}
	if (comma_count != 2)
		return (free(stock), print_err(NULL, "Invalid Color", 1));
	if (fill_color_values(parse, stock, *line) == 1)
		return (free(stock), 1);
	free(stock);
	return (0);
}
