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
void	process1(int fd1, int fd2, char *cmd1, char **envp, char **argv);
void	process2(int fd1, int fd2, char *cmd2, char **envp, char **argv);
char	*get_cmd_path(char *cmd, char **envp);

#endif
