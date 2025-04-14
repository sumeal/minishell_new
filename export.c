/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muzz <muzz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:01:17 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/14 22:45:15 by muzz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_variable(char **args, char ***envp, int *g_exit_status)
{
	int	ret_val;

	if (!args[1])
	{
		print_sorted_envp(*envp);
		*g_exit_status = 0;
	}
	else
	{
		ret_val = add_variable_to_env(envp, args);
		if (ret_val)
			*g_exit_status = 0;
	}
	return (1);
}

void	print_sorted_envp(char **envp)
{
	char	**copy;
	int		i;

	copy = duplicate_env_array(envp);
	if (!copy)
		return ;
	sort_env(copy);
	i = 0;
	while (copy[i])
	{
		if (ft_strncmp(copy[i], "_=", 2) != 0)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(copy[i], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		i++;
	}
	free_path(copy);
}

char	**duplicate_env_array(char **envp)
{
	char	**copy;
	int		i;
	int		count;

	i = 0;
	count = count_envp(envp);
	copy = (char **)malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	while (i < count)
	{
		copy[i] = change_format(envp[i]);
		if (!copy[i])
		{
			free_path(copy);
			return (NULL);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

void	sort_env(char **envp)
{
	int		i;
	int		j;
	int		count;
	char	*name1;
	char	*name2;

	count = count_envp(envp);
	i = -1;
	while (++i < count - 1)
	{
		j = -1;
		while (++j < count - i - 1)
		{
			if (skip(envp[j], envp[j + 1]) == 0)
			{
				name1 = get_var_name(envp[j]);
				name2 = get_var_name(envp[j + 1]);
				if (name1 && name2 && ft_strcmp(name1, name2) > 0)
					swap_envp(envp, j, j + 1);
				free(name1);
				free(name2);
			}
		}
	}
}

void	swap_envp(char **envp, int i, int j)
{
	char	*temp;

	temp = envp[i];
	envp[i] = envp[j];
	envp[j] = temp;
}
