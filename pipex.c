#include "pipex.h"

void	ft_error(char *error_msg)
{
	perror(error_msg);
	exit(EXIT_FAILURE);
}

void	process1(int fd1, int fd2, char *cmd1, char **envp)
{
	int		fd[2];
	pid_t	pid1;

	if (pipe(fd) < 0)
		ft_error("Pipe error");
	pid1 = fork();
	if (pid1 < 0)
		ft_error("Fork error");
	if (pid1 == 0)
	{
		close(fd[0]);
		dup2(fd1, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd1);
		close(fd[1]);
		execve(cmd1, NULL, envp);
		ft_error("Execve error in process1");
	}
	else
	{
		close(fd[1]);
		waitpid(pid1, NULL, 0);
	}
}

void	process2(int fd1, int fd2, char *cmd2, char **envp)
{
	int		fd[2];
	pid_t	pid2;

	if (pipe(fd) < 0)
		ft_error("Pipe error");
	pid2 = fork();
	if (pid2 < 0)
		ft_error("Fork error");
	if (pid2 == 0)	
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		dup2(fd2, STDIN_FILENO);
		close(fd2);
		close(fd[1]);
		execve(cmd2, NULL, envp);
		ft_error("Execve error in process2");
	}
	else
	{
		close(fd[1]);
		waitpid(pid2, NULL, 0);
	}
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*path;
	char	*full_cmd;
	char	**paths;
	int		i;

	paths = ft_split(getenv("PATH"), ':');
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


int	main(int argc, char **argv, char **envp)
{
	int		fd1;
	int		fd2;
	int		pid1;
	int		pid2;


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

}
