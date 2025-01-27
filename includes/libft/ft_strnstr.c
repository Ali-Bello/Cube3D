/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbaraka <elbaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 22:03:39 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/21 21:40:46 by elbaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strnstr(const char *src, const char *to_find, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (ft_strlen(to_find) > ft_strlen(src))
		return (NULL);
	else if (to_find[j] == '\0')
		return ((char *)src);
	else if (n == 0)
		return (NULL);
	while (src[i + j] && i + j < n)
	{
		j = 0;
		while (src[i + j] == to_find[j] && i + j < n)
		{
			if (to_find[j + 1] == '\0')
				return ((char *)src + i);
			j++;
		}
		i++;
	}
	return (NULL);
}
