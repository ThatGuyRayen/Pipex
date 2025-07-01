#include "pipex.h"

void	ft_error(char *error_txt)
{
	perror(error_txt);
	exit(EXIT_FAILURE);
}

void	process_child(char **argv, char **envp, int *fd)
{
	int	infile;

	infile = open(argv[1], O_RDONLY, 0777);
	if (infile == -1)
		ft_error("Can't open Infile");
	dup2(fd[1], STDOUT_FILENO);
	dup2(infile, STDIN_FILENO);
	close(fd[0]);
	run_it(argv[2], envp);
}

void	process_main(char **argv, char **envp, int *fd)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		ft_error("ERROR with Outfile");
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(fd[1]);
	run_it(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1;

	if (argc ==5)
	{
		if (pipe(fd) == -1)
			ft_error("Invalid Fd");
		pid1 = fork();
		if (pid1 == -1)
			ft_error("Can't Fork");
		if (pid1 == 0)
			process_child(argv, envp, fd);
		waitpid(pid1, NULL, 0);
		process_main(argv, envp, fd);
		}
	else
		ft_error("Terrible argumentssss");
	return (0);
}
