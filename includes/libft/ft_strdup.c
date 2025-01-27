/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 16:24:37 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/23 01:37:11 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strdup(const char *src)
{
	char	*s2;
	int		i;

	s2 = (char *)malloc(sizeof(char) * ft_strlen(src) + 1);
	i = 0;
	if (!s2)
		return (0);
	while (*src)
	{
		s2[i++] = *src++;
	}
	s2[i] = '\0';
	return (s2);
}
