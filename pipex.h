#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

void	ft_error(char *error_msg);
char	*ft_getenv(char *name);
void	process1(int infile, int pipe_write, char *cmd_str, char **envp);
void	process(int pipe_read, int outfile, char *cmd_str, char **envp);
char	*get_cmd_path(char *cmd, char **envp);

#endif
