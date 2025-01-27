/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbaraka <elbaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 22:03:43 by elbaraka          #+#    #+#             */
/*   Updated: 2023/11/21 06:02:05 by elbaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s = src;

	d = dest;
	if (d == s)
		return (dest);
	if (s < d && s + len > d)
	{
		s += len;
		d += len;
		while (len--)
			*(--d) = *(--s);
	}
	else
	{
		while (len--)
			*(d++) = *(s++);
	}
	return (dest);
}
