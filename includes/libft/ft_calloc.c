/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbaraka <elbaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 14:36:40 by elbaraka          #+#    #+#             */
/*   Updated: 2025/01/21 18:33:07 by elbaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*res;

	if (size && count > 65535 / size)
		return (NULL);
	res = malloc(count * size);
	if (!res)
	{
		free(res);
		return (NULL);
	}
	ft_bzero(res, count * size);
	return (res);
}
