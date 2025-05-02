/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchen2 <jchen2@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:05:13 by jchen2            #+#    #+#             */
/*   Updated: 2024/11/15 12:27:12 by jchen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
avoid buffer overflows by taking the destination buffer size as a parameter
ensures that the destination string is always null-terminated 
*/
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (src[i] != '\0')
		i++;
	if (size == 0)
		return (i);
	j = 0;
	while (j < size - 1 && src[j] != '\0')
	{
		dst[j] = src[j];
		j++;
	}
	dst[j] = '\0';
	return (i);
}
/*
#include <stdio.h>

int main()
{
	char dest[10];
	const char *src = "Hello, World!";

	int len = ft_strlcpy(dest, src, sizeof(dest));
	printf("%d\n", len);
	printf("%s\n", dest);
	return (0);
}
*/