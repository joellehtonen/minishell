/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 10:28:35 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/07 15:10:22 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	totallen;
	size_t	i;
	size_t	j;
	size_t	dstlen;

	i = 0;
	j = 0;
	totallen = ft_strlen(dst) + ft_strlen(src);
	dstlen = ft_strlen(dst);
	if (dstsize < dstlen)
		return (dstsize + ft_strlen(src));
	while (dst[i])
		i++;
	while (i + 1 < dstsize && *src)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (totallen);
}
