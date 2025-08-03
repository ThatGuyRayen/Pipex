/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:50:05 by tpandya           #+#    #+#             */
/*   Updated: 2025/07/01 11:50:06 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_cmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd && cmd[i])
		free(cmd[i++]);
	free(cmd);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*temp;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, F_OK) == 0)
		{
			free_cmd(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_cmd(paths);
	return (NULL);
}

void	run_it(char *argv, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(argv, ' ');
	if (!cmd || !cmd[0])
	{
		free_cmd(cmd);
		ft_error("Error while splitting command");
	}
	path = get_cmd_path(cmd[0], envp);
	if (!path)
	{
		free_cmd(cmd);
		ft_error("Error: command not found");
	}
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		free_cmd(cmd);
		ft_error("Error executing command");
	}
}
