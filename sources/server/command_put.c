/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_put.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 22:46:43 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/19 00:40:37 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int		command_put(int client, char *cmd, char **argv)
{
	int		fd;

	(void)cmd;
	if (ft_tablen(argv) != 2)
	{
		send_data(client, "Invalid arguments.", ft_strlen("Invalid arguments."));
		return (501);
	}
	fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	write(fd, argv[2], ft_strlen(argv[2]));

	send_data(client, NULL, 0);
	return (200);
}
