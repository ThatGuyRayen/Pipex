#include "pipex.h"

void	ft_error(char *error_msg)
{
	perror(error_msg);
	exit(EXIT_FAILURE);
}

void	free_whole(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		free(str[i++]);
	free(str);
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
	char	*full_cmd;
	char	*joined;
	char	**paths;
	char	**tmp;

	(void)envp;
	paths = ft_split(ft_getenv("PATH"), ':');
	if (!paths)
		ft_error("Error splitting PATH");
	tmp = paths;
	while (*tmp)
	{
		joined = ft_strjoin(*tmp, "/");
		full_cmd = ft_strjoin(joined, cmd);
		free(joined);
		if (access(full_cmd, X_OK) == 0)
			return (free_whole(paths), full_cmd);
		free(full_cmd);
		tmp++;
	}
	free_whole(paths);
	ft_error("Command not found");
	return (NULL);
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
		free(cmd_path);
		free_whole(cmd_args);
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
		free(cmd_path);
		free_whole(cmd_args);
		ft_error("Execve failed for cmd2");
	}
}

void	close_stuff(int infile, int outfile, int pipe_read, int pipe_write)
{
	close(infile);
	close(outfile);
	close(pipe_read);
	close(pipe_write);
}

int	main(int argc, char **argv, char **envp)
{
	int	infile;
	int	outfile;
	int	pipe_fd[2];

	if (argc != 5)
	{
		ft_error("Usage: ./pipex infile cmd1 cmd2 outfile");
	}

	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		ft_error("Error opening infile");

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
	{
		close(infile);
		ft_error("Error opening outfile");
	}

	if (pipe(pipe_fd) < 0)
	{
		close(infile);
		close(outfile);
		ft_error("Pipe creation failed");
	}

	process1(infile, pipe_fd[1], argv[2], envp);
	process2(pipe_fd[0], outfile, argv[3], envp);

	close_stuff(infile, outfile, pipe_fd[0], pipe_fd[1]);

	wait(NULL); // Wait for child processes to finish
	wait(NULL);

	return (0);
}
