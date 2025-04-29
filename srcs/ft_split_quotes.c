/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abin-moh <abin-moh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:18:47 by jchen2            #+#    #+#             */
/*   Updated: 2025/04/29 15:09:15 by abin-moh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	flag_func(char c, int *num, t_bool *flag)
{
	if (c == '\'' && !flag->d_s)
		flag->s_s = !flag->s_s;
	else if (c == '\"' && !flag->s_s)
		flag->d_s = !flag->d_s;
	else if ((c != ' ' && c != '\n' && c != '\t') && !flag->w_s)
	{
		(*num)++;
		flag->w_s = true;
	}
	else if ((c == ' ' || c == '\n' || c == '\t') && !flag->s_s && !flag->d_s)
	{
		flag->w_s = false;
	}
}

static int	num_substr(char *str)
{
	int		num;
	t_bool	flag;

	num = 0;
	flag.w_s = false;
	flag.s_s = false;
	flag.d_s = false;
	while (*str != '\0')
	{
		flag_func(*str, &num, &flag);
		str++;
	}
	printf("num is %d\n", num);
	return (num);
}

static char	**free_rest(char **arr, int i)
{
	i = i - 1;
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free (arr);
	return (NULL);
}

static char	**set_substr(char *str, int num)
{
	char	**arr;
	int		i;
	t_bool	flag;

	arr = (char **)malloc((num + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	flag.d_s = false;
	flag.s_s = false;
	flag.w_s = false;
	while (*str)
	{
		while (*str == ' ' || *str == '\n' || *str == '\t')
			str++;
		if (*str)
		{
			arr[i] = count_copy_substr(&str, &flag);
			if (!arr[i])
				return (free_rest(arr, i));
			i++;
		}
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split_quotes(char *str)
{
	int		num;
	char	**arr;

	if (!str)
		return (NULL);
	num = num_substr(str);
	arr = set_substr(str, num);
	return (arr);
}
