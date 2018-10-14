/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:47:25 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/12 20:28:42 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft.h"

# include <stdio.h>

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

# include <netdb.h>
# include <netinet/in.h>

# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/stat.h>

# include <dirent.h>

char	**ft_tabstr(char **tab, char *entry);
void	ft_tabdel(char ***tab);
size_t	ft_tablen(char **tab);

size_t	read_file(const int fd, char **line);

int		init_server(char **envp);

int		send_data(int client, const void *data, size_t len);

int		command_handler(int client, char *cmd);
int		command_cd(int client, char **argv);

extern char	*g_root;
extern char	*g_home;
extern char	*g_out;
#endif
