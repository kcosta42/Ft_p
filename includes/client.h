/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:47:39 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/15 13:09:38 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "libft.h"

# include <stdio.h>

# include <stdlib.h>
# include <unistd.h>

# include <netdb.h>
# include <netinet/in.h>

# include <arpa/inet.h>

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>

# include <termios.h>

typedef struct	s_lcmd_hash
{
	uint8_t	cmd_len;
	char	*cmd;
	char	*bin;
	int		(*exec)(char **, char *, char **);
}				t_lcmd_hash;

int		lcommand_handler(char **env, char *cmd);
int		lcommand_cd(char **env, char *cmd, char **argv);

#endif
