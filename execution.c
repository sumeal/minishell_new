/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muzz <muzz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:51:58 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/14 22:53:14 by muzz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_child_processes(t_cmd *cmd, t_exec_cmd *vars,
			int *g_exit_status, char ***envp)
{
	t_cmd	*cur;

	vars->i = 0;
	cur = cmd;
	while (vars->i < (*vars).cmd_count)
	{
		(*vars).pid[vars->i] = fork();
		setup_signal_child();
		if (vars->pid[vars->i] < 0)
		{
			perror("fork");
			return ;
		}
		if (vars->pid[vars->i] == 0)
		{
			execute_command_in_child(cur, vars, g_exit_status, envp);
		}
		if (cur->next)
			cur = cur->next;
		vars->i++;
	}
}

void	free_pipe(t_exec_cmd *vars)
{
	int	i;

	if (!vars || !vars->pipefd)
		return;

	i = 0;
	while (i < vars->cmd_count)
	{
		if (vars->pipefd[i] != NULL)
		{
			free(vars->pipefd[i]);
			vars->pipefd[i] = NULL;
		}
		i++;
	}
	free(vars->pipefd);
	vars->pipefd = NULL;
}


void	run_the_commands(t_cmd *cmd, int *g_exit_status,
			t_exec_cmd *vars, char ***envp)
{
	vars->pid = malloc(sizeof(pid_t) * vars->cmd_count);
	if (!vars->pid)
	{
		perror("pid");
		exit(1);
	}
	save_original_fd(vars);
	create_child_processes(cmd, vars, g_exit_status, envp);
	if (vars->cmd_count != 1)
		closing_pipes(&vars);
	wait_for_child(vars);
	set_exit_status(vars, g_exit_status);
	restore_original_fd(vars);
	free(vars->pid);
	free_pipe(vars);
}

void	execute_multiple_commands(t_cmd **cmd,
	char ***envp, int *g_exit_status)
{
	t_exec_cmd	vars;

	vars.cmd_count = count_command(*cmd);
	vars.pipefd = malloc(sizeof(int *) * vars.cmd_count);
	vars.i = 0;
	while (vars.i < vars.cmd_count)
	{
		vars.pipefd[vars.i] = malloc(sizeof(int) * 2);
		if (pipe(vars.pipefd[vars.i]) == -1)
		{
			perror("pipe");
			return ;
		}
		vars.i++;
	}
	run_the_commands(*cmd, g_exit_status, &vars, envp);
}

void	execution(t_cmd *commands, char ***mini_envp, int *g_exit_status)
{
	int			fok;
	t_exec_cmd	vars;

	if (commands->next == NULL && !commands->append
		&& !commands->input_file && !commands->output_file)
	{
		fok = execute_builtin_command(&commands, mini_envp, g_exit_status);
		if (fok == 0)
		{
			vars.cmd_count = 1;
			run_the_commands(commands, g_exit_status, &vars, mini_envp);
		}
	}
	else
		execute_multiple_commands(&commands, mini_envp, g_exit_status);
}

int	execute_builtin_command(t_cmd **cmd,
	char ***envp, int *g_exit_status)
{
	if (ft_strcmp((*cmd)->argv[0], "echo") == 0
		&& (*cmd)->argv[1]
		&& ft_strcmp((*cmd)->argv[1], "$?") == 0)
		return (print_exit_status(g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "cd") == 0 && (*cmd)->prev == NULL
		&& (*cmd)->next == NULL)
	{
		change_directory(cmd, envp, g_exit_status);
		return (1);
	}
	else if (ft_strcmp((*cmd)->argv[0], "echo") == 0)
		return (print_echo((*cmd)->argv, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "env") == 0)
		return (print_env(cmd, *envp, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "pwd") == 0)
		return (print_pwd(*envp, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "export") == 0)
		return (export_variable((*cmd)->argv, envp, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "unset") == 0)
		return (unset_env((*cmd), *envp, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "exit") == 0)
		return (exit_function((*cmd), *envp, g_exit_status));
	return (0);
}
