/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abin-moh <abin-moh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:12:22 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/14 16:42:00 by abin-moh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

char	**copy_envp(char **envp)
{
	char	**copy;
	int		i;

	i = count_envp(envp);
	copy = (char **)malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = -1;
	while (envp[++i])
		copy[i] = ft_strdup(envp[i]);
	copy[i] = NULL;
	return (copy);
}

char	*get_shlvl_value(char **envp, int index)
{
	int		number1;
	int		position;
	char	*number2;
	char	*ret;
	
	position = 0;
	while (envp[index][position] && envp[index][position] != '=')
		position++;
	position++;
	number1 = ft_atoi((envp[index]) + position);
	number1++;
	number2 = ft_itoa(number1);
	ret = ft_strdup(number2);
	return (ret);
}

void	increment_shlvl(char ***envp)
{
	int		index;
	char	*new;
	char	*temp;
	char	*value_int;

	index = find_variable("SHLVL", *envp);
	if (index >= 0)
		value_int = get_shlvl_value(*envp, index);
	else
		return ;
	temp = ft_strdup("SHLVL=");
	new = ft_strjoin(temp, value_int);
	free(temp);
	free(value_int);
	if (!new)
		return ;
	if (index >= 0)
	{
		free((*envp)[index]);
		(*envp)[index] = new;
	}
}

int main(int argc, char **argv, char **envp)
{
	char			**mini_envp;
	char			*input;
	t_cmd			*commands;
	struct termios	original_term;
	struct termios	new_term;
	int				g_exit_status;

	g_signal = 0;
	(void)argc;
	(void)argv;
	g_exit_status = 0;
	mini_envp = copy_envp(envp);
	increment_shlvl(&mini_envp);
	while (1)
	{
		setup_signal_handlers(&original_term, &new_term);
		input = readline("minishell> ");
		if (g_signal == 130)
		{
			g_exit_status = g_signal;
			g_signal = 0;
		}
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		commands = syntactic_analysis(input, mini_envp, &g_exit_status);
		if (commands)
		{
			// while(commands)
			// {
			// 	print_cmd(commands);
			// 	commands = commands->next;
			// }
			execution(commands, &mini_envp, &g_exit_status);
			free_cmds(commands);
		}
		free(input);
	}
	tcsetattr(STDERR_FILENO, TCSANOW, &original_term);
	free_path(mini_envp);
	rl_clear_history();
	return (g_exit_status);
}
