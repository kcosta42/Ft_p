/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:47:25 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/19 14:55:35 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

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
# include <sys/stat.h>

# include <sys/mman.h>

# include <dirent.h>

typedef struct	s_cmd_hash
{
	uint8_t	cmd_len;
	char	*cmd;
	char	*bin;
	int		(*exec)(int, char *, char **);
}				t_cmd_hash;

int		init_server(char **envp);

int		send_data(int client, const void *data, size_t size, int reply);

int		command_handler(int client, char *cmd);
int		command_cd(int client, char *cmd, char **argv);
int		command_get(int client, char *cmd, char **argv);
int		command_put(int client, char *cmd, char **argv);

extern char	*g_root;
extern char	*g_home;
extern char	*g_out;
extern char	*g_pwd;

#endif
