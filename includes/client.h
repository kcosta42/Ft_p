/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:47:39 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/19 12:05:08 by kcosta           ###   ########.fr       */
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

# include <termios.h>

typedef struct	s_lcmd_hash
{
	uint8_t	cmd_len;
	char	*cmd;
	char	*bin;
	int		(*exec)(char **, char *, char **);
}				t_lcmd_hash;

int		lcommand_handler(int socket, char *cmd, char **env);
int		lcommand_cd(char **env, char *cmd, char **argv);
int		lcommand_put(int socket, char **argv);

int		send_command(int socket, char *buffer);
int		receive_data(int socket);

extern char	*last_cmd;

#endif
