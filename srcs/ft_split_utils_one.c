/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils_one.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abin-moh <abin-moh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:16:32 by jchen2            #+#    #+#             */
/*   Updated: 2025/04/29 15:09:28 by abin-moh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_quote_char(char **str, t_bool *flag, char *buf, int *len)
{
	if ((**str == '\'' && !flag->d_s) || (**str == '\"' && !flag->s_s))
	{
		if (**str == '\'')
			flag->s_s = !flag->s_s;
		else
			flag->d_s = !flag->d_s;
		buf[(*len)++] = **str;
		(*str)++;
		return (1);
	}
	return (0);
}

char	*count_copy_substr(char **str, t_bool *flag)
{
	char	*buf;
	int		len;

	buf = malloc(ft_strlen(*str) + 1);
	if (!buf)
		return (NULL);
	len = 0;
	while (**str)
	{
		if (process_quote_char(str, flag, buf, &len))
			continue ;
		if ((**str == ' ' || **str == '\n' || **str == '\t')
			&& !flag->s_s && !flag->d_s)
			break ;
		buf[len++] = **str;
		(*str)++;
	}
	buf[len] = '\0';
	return (buf);
}
