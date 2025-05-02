/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchen2 <jchen2@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 17:04:10 by jchen2            #+#    #+#             */
/*   Updated: 2024/11/13 09:57:01 by jchen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	count the size of list.
	return the size.
*/
#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	len;

	len = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		len++;
	}
	return (len);
}
/*
#include <stdio.h>
#include <stdlib.h>

t_list	*ft_lstnew(void *content)
{
	t_list	*lstnew;

	lstnew = (t_list *)malloc(sizeof(t_list));
	if (!lstnew)
		return (NULL);
	lstnew->content = content;
	lstnew->next = NULL;
	return (lstnew);
}

int	main(void)
{
	t_list	*one = ft_lstnew("hello world");
	t_list	*two = ft_lstnew("I am CJ");
	t_list	*three = ft_lstnew("how are you!");
	int	size;

	one->next = two;
	two->next = three;

	size = ft_lstsize(one);
	printf("size: %d\n", size);
	free(one);
	free(two);
	free(three);
	return (0);
}
*/