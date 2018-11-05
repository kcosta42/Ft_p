/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:47:39 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 13:47:49 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "libft.h"

# include <stdio.h>

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>

# include <netdb.h>
# include <netinet/in.h>

# include <arpa/inet.h>

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>

# include <sys/mman.h>

# define MSG_DEFAULT "SUCCESS."
# define MSG_200 "SUCCESS: Command okay."
# define MSG_221 "SUCCESS: Service closing control connection."
# define MSG_500 "ERROR: Syntax error, command unrecognized."
# define MSG_501 "ERROR: Syntax error in parameters or arguments."

typedef struct	s_lcmd_hash
{
	uint8_t		cmd_len;
	char		*cmd;
	char		*bin;
	int			(*exec)(char **, char *, char **);
}				t_lcmd_hash;

int				exec_command(char **envp, char *cmd, char **argv);
int				lcommand_handler(int socket, char *cmd, char **env);
int				lcommand_put(int socket, char **argv);
int				lcommand_cd(char **env, char *cmd, char **argv);

int				isvalid_path(char *path, char *arg);
char			**path_split(char *absolute_path);
char			*get_path(char **path);

int				send_command(int socket, char *buffer);
int				receive_data(int socket);

extern char		*g_last_cmd;

#endif
