/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchen2 <jchen2@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 16:38:50 by jchen2            #+#    #+#             */
/*   Updated: 2024/12/18 16:40:34 by jchen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 add the node into the list in front of head node
*/
#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new || !lst)
		return ;
	new->next = *lst;
	*lst = new;
}
/*
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

#include <stdio.h>

int	main(void)
{
	t_list *head = NULL;
		t_list *node1 = ft_lstnew("first node");
	t_list *node2 = ft_lstnew("second node");

	ft_lstadd_front(&head, node1);
	ft_lstadd_front(&head, node2);
	t_list *current = head;
	while (current)
	{
		printf("node front: %s\n", (char *)current->content);
		current = current->next;
	}
	free(node1);
	free(node2);
	return (0);
}
*/