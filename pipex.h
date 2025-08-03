/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:50:31 by tpandya           #+#    #+#             */
/*   Updated: 2025/07/01 11:50:33 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void	free_cmd(char **cmd);
char	*get_cmd_path(char *cmd, char **evnp);
void	run_it(char *argv, char **envp);

#endif
