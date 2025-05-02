/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchen2 <jchen2@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:00:43 by jchen2            #+#    #+#             */
/*   Updated: 2024/11/15 12:27:55 by jchen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
conect to string, return a new one
*/
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*ptr_star;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	while (s1[i] != '\0')
		i++;
	while (s2[j] != '\0')
		j++;
	str = (char *)malloc(i + j + 1);
	if (!str)
		return (NULL);
	ptr_star = str;
	while (*s1)
		*str++ = *s1++;
	while (*s2)
		*str++ = *s2++;
	*str = '\0';
	return (ptr_star);
}

/*
#include <stdio.h>

int main()
{
	char	*s1 = "hello ";
	char	*s2 = "world";
	char	*str_join = ft_strjoin(s1, s2);
	printf("%s\n", str_join);
	free(str_join);
	return(0);
}
*/
