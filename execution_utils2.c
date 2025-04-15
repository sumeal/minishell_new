/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abin-moh <abin-moh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:36:19 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/15 16:18:10 by abin-moh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error(char *s, int exit)
{
	ft_putstr_fd("bash: ", 2);
	perror(s);
	return (exit);
}

void	free_path(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	*get_path(char *cmd, char **envp)
{
	char	*part_path;
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = -1;
	while (paths[++i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (!path)
			break ;
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
	}
	free_path(paths);
	return (NULL);
}

int	setup_input(t_cmd *cmd, t_exec_cmd *vars, int *g_exit_status)
{
	if (cmd->input_file)
	{
		vars->fdin = open(cmd->input_file, O_RDONLY);
		if (vars->fdin < 0)
		{
			*g_exit_status = 1;
			return (print_error(cmd->input_file, -1));
		}
	}
	else if (cmd->hd_delimeter)
	{
		vars->fdin = hd_printf(cmd->hd_delimeter);
		if (vars->fdin < 0)
		{
			*g_exit_status = 130;
			return (-1);
		}
	}
	else
		vars->fdin = dup(vars->ori_in);
	dup2(vars->fdin, STDIN_FILENO);
	close(vars->fdin);
	return (0);
}

int	handle_last_command_output(t_cmd *cmd, t_exec_cmd *vars)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	vars->fdout = open(cmd->output_file, flags, 0644);
	if (vars->fdout < 0)
		return (print_error(cmd->output_file, -1));
	dup2(vars->fdout, STDOUT_FILENO);
	close(vars->fdout);
	return (0);
}
