#include "pipex.h"

void	ft_error(char *error_msg)
{
	perror(error_msg);
	exit(EXIT_FAILURE);
}

char	*ft_getenv(char *name)
{
	extern char	**environ;
	char		**env;
	char		*value;

	env = environ;
	while (*env)
	{
		if (ft_strncmp(*env, name, ft_strlen(name)) == 0
			&& (*env)[ft_strlen(name)] == '=')
		{
			value = *env + ft_strlen(name) + 1; // Skip the '='
			return (value);
		}
		env++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*path;
	char	*full_cmd;
	char	**paths;
	int		i;

	paths = ft_split(ft_getenv("PATH"), ':');
	if (!paths)
		ft_error("Error splitting PATH");
	while (*paths)
	{
		path = *paths;
		full_cmd = ft_strjoin(path, "/");
		if (!full_cmd)
			ft_error("Error joining path");
		full_cmd = ft_strjoin(full_cmd, cmd);
		if (!full_cmd)
			ft_error("Error joining command");
		if (access(full_cmd, X_OK) == 0)
		{
			free(paths);
			return (full_cmd);
		}
		free(full_cmd);
		paths++;
	}
	free(paths);
	ft_error("Command not found");
	return (NULL); // This line will never be reached
}

void	process1(int fd1, int fd2, char *cmd1, char **envp, char **argv)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		ft_error("Fork failed");
	if (pid == 0) // Child process
	{
		dup2(fd1, STDIN_FILENO);
		dup2(fd2, STDOUT_FILENO);
		close(fd1);
		close(fd2);
		execve(cmd1, argv, envp);
		ft_error("Execve failed for cmd1");
	}
	else // Parent process
	{
		waitpid(pid, NULL, 0);
	}
}

void	process2(int fd1, int fd2, char *cmd2, char **envp, char **argv)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		ft_error("Fork failed");
	if (pid == 0) // Child process
	{
		dup2(fd1, STDIN_FILENO);
		dup2(fd2, STDOUT_FILENO);
		close(fd1);
		close(fd2);
		execve(cmd2, argv, envp);
		ft_error("Execve failed for cmd2");
	}
	else // Parent process
	{
		waitpid(pid, NULL, 0);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		fd1;
	int		fd2;
	int		pid1;
	int		pid2;
	char	*cmd1;
	char	*cmd2;

	if (argc < 5)
		ft_error("Usage: ./pipex file1 cmd1 cmd2 file2");
	fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0)
		ft_error("Error opening file1");
	fd2 = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd2 < 0)
	{
		close(fd1);
		ft_error("Error opening file2");
	}
	cmd1 = get_cmd_path(argv[2], envp);
	cmd2 = get_cmd_path(argv[3], envp);
	if (!cmd1 || !cmd2)
	{
		close(fd1);
		close(fd2);
		ft_error("Command not found");
	}
	process1(fd1, fd2, cmd1, envp, argv + 2);
	process2(fd1, fd2, cmd2, envp, argv + 3);
	free(cmd1);
	free(cmd2);
	close(fd1);
	close(fd2);
	return (0);
}
