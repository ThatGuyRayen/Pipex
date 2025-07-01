#include "pipex.h"

char	*get_cmd_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*temp;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (0);
}

void	run_it(char *argv, char **envp)
{
	char **cmd;
	int		i;
	char	*path;

	i = 0;
	cmd = ft_split(argv, ' ');
	path = get_cmd_path(cmd[0], envp);
	if (!path)
	{
		while (cmd[i])
		{
			free(cmd);
			i++;
		}
		free(cmd);
		ft_error("Error while running given Command");
	}
	if(execve(path, cmd, envp) == -1)
		ft_error("Invalid Command or something else with 'cmd' fs");
}


