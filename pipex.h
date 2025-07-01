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
void	process_main(char **argv, char **envp, int *fd);
void	process_child(char **argv, char **envp, int *fd);
char	*get_cmd_path(char *cmd, char **evnp);
void	run_it(char *argv, char **envp);

#endif
