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
	{
		free(paths);
		ft_error("Error splitting PATH");
	}
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

void	process1(int infile, int pipe_write, char *cmd_str, char **envp)
{
	int	pid;
	char	**cmd_args;
	char	*cmd_path;

	pid = fork();
	if (pid < 0)
		ft_error("Fork failed");
	if (pid == 0) // Child process
	{
		dup2(infile, STDIN_FILENO);
		dup2(pipe_write, STDOUT_FILENO);
		close(infile);
		close(pipe_write);
		cmd_args = ft_split(cmd_str, ' ');
		cmd_path = get_cmd_path(cmd_args[0], envp);
		execve(cmd_path, cmd_args, envp);
		ft_error("Execve failed for cmd1");
	}	
}

void	process2(int pipe_read, int outfile, char *cmd_str, char **envp)
{
	int	pid;
	char	**cmd_args;
	char	*cmd_path;

	pid = fork();
	if (pid < 0)
		ft_error("Fork failed");
	if (pid == 0) // Child process
	{
		dup2(pipe_read, STDIN_FILENO);
		dup2(outfile, STDOUT_FILENO);
		close(pipe_read);
		close(outfile);
		cmd_args = ft_split(cmd_str, ' ');
		cmd_path = get_cmd_path(cmd_args[0], envp);
		execve(cmd_path, cmd_args, envp);	
		ft_error("Execve failed for cmd2");
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int	infile;
	int	outfile;

	if (argc != 5)
	{
		ft_error("Usage: ./pipex infile cmd1 cmd2 outfile");
	}
	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		ft_error("Error opening infile");
	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		ft_error("Error opening outfile");
	if (pipe(pipefd) < 0)
		ft_error("Pipe failed");
	process1(infile, pipefd[1], argv[2], envp);
	process2(pipefd[0], outfile, argv[3], envp);
	close(pipefd[0]);
	close(pipefd[1]);
	close(infile);
	close(outfile);
	wait(NULL);
	wait(NULL);
	return (0);
}
