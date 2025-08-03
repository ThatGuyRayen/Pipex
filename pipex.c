/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:49:42 by tpandya           #+#    #+#             */
/*   Updated: 2025/07/01 11:49:43 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	dup2(infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]); 
	close(infile); 
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
	close(fd[0]); 
	close(outfile);
	run_it(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1;

	if (argc == 5)
	{
		if (pipe(fd) == -1)
			ft_error("Invalid Fd");
		pid1 = fork();
		if (pid1 == -1)
			ft_error("Can't Fork");
		if (pid1 == 0)
			process_child(argv, envp, fd);
		close(fd[1]); 
		waitpid(pid1, NULL, 0);
		process_main(argv, envp, fd);
	}
	else
		ft_error("Terrible argumentssss");
	return (0);
}
