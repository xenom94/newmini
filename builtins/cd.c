/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/28 21:31:07 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	home(char *oldpwd)
{
	char	*home_path;

	home_path = getenv("HOME");
	if (home_path)
	{
		chdir(home_path);
		update_env_variable(g_vars.env, "OLDPWD", oldpwd);
		update_env_variable(g_vars.env, "PWD", home_path);
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_vars.exit_status = 1;
	}
}

void	with_path(char **argv)
{
	char	*new_path;

	init_current_dir();
	new_path = build_path(g_vars.current_dir, argv[1]);
	if (g_vars.current_dir)
		update_env_variable(g_vars.env, "OLDPWD=", g_vars.current_dir);
	if (chdir(new_path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_vars.exit_status = 1;
		free(new_path);
		return ;
	}
	update_current_dir(new_path);
	free(new_path);
}

void	cd(t_command *cmd)
{
	char	*current;

	g_vars.exit_status = 0;
	if (!g_vars.current_dir)
		init_current_dir();
	current = get_current_dir();
	if (current)
	{
		update_env_variable(g_vars.env, "OLDPWD=", current);
		free(current);
	}
	if (cmd->arg_count == 1)
		home(g_vars.saved_oldpwd);
	else if (cmd->arg_count == 2)
	{
		if (ft_strcmp(cmd->args[1], "-") == 0)
			go_oldpwd();
		else
			with_path(cmd->args);
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		g_vars.exit_status = 1;
	}
}

void	go_oldpwd(void)
{
	if (!g_vars.saved_oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		g_vars.exit_status = 1;
		return ;
	}
	if (g_vars.current_dir)
		update_env_variable(g_vars.env, "OLDPWD=", g_vars.current_dir);
	if (chdir(g_vars.saved_oldpwd) == -1)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not accessible\n", 2);
		g_vars.exit_status = 1;
		return ;
	}
	update_current_dir(g_vars.saved_oldpwd);
	ft_putstr_fd(g_vars.saved_oldpwd, 1);
	ft_putstr_fd("\n", 1);
}
