/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbaraka <elbaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 00:18:37 by elbaraka          #+#    #+#             */
/*   Updated: 2023/11/21 06:02:18 by elbaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

int	ft_memcmp(const void *b1, const void *b2, size_t size)
{
	size_t			i;
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)b1;
	p2 = (unsigned char *)b2;
	i = 0;
	if (size == 0)
		return (0);
	while (p1[i] == p2[i] && i < size - 1)
	{
		i++;
	}
	return ((int)p1[i] - p2[i]);
}
