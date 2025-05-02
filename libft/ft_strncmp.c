/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchen2 <jchen2@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:05:32 by jchen2            #+#    #+#             */
/*   Updated: 2024/11/15 12:26:20 by jchen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
compares up to a specified number of characters between two strings
return value is the difference of both character 
*/
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>

int main()
{
	printf("%d\n", ft_strncmp("hello", "hello world", 5));
	printf("%d\n", ft_strncmp("hello", "hello", 4));
	printf("%d\n", ft_strncmp("adc", "fde", 3));
}
*/