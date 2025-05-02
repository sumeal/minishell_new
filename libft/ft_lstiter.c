/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchen2 <jchen2@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:17:01 by jchen2            #+#    #+#             */
/*   Updated: 2024/11/13 10:08:04 by jchen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	iterate the node on the list
	deal with node's content using func
*/
#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL || f == NULL)
		return ;
	while (lst != NULL)
	{
		f(lst->content);
		lst = lst->next;
	}
}
/*
#include <stdio.h>
#include <stdlib.h>

void	printcontent(void *content)
{
	printf("%s\n", (char *)content);
}

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

	one->next = two;
	two->next = three;

	ft_lstiter(one, printcontent);
	free(one);
	free(two);
	free(three);
	return (0);
}
*/