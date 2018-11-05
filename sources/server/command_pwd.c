/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 15:50:46 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 15:54:19 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*g_out;

void	manage_data(int client, int fd)
{
	char		*data;
	size_t		data_length;
	char		*tmp;

	data_length = ft_readfile(fd, &data);
	if (data_length == (size_t)-1)
		send_data(client, data, 0, 200);
	else
	{
		tmp = ft_strdup(data + ft_strlen(g_root));
		ft_strdel(&data);
		data = ft_strdup(*tmp ? tmp : "/");
		send_data(client, data, ft_strlen(data), 200);
		ft_strdel(&tmp);
	}
	ft_strdel(&data);
}

int		command_pwd(int client, char *cmd, char **argv)
{
	pid_t		pid;
	int			fd;
	size_t		len;

	len = ft_strlen("pwd: too many arguments");
	if (ft_tablen(argv) != 1)
		return (send_data(client, "pwd: too many arguments", len, 501));
	ft_strdel(argv);
	argv[0] = ft_strdup(cmd);
	pid = fork();
	fd = open(g_out, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!pid)
	{
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		execv(cmd, (char *const*)argv);
		exit(1);
	}
	if (pid > 0)
		wait4(0, NULL, 0, NULL);
	manage_data(client, fd);
	close(fd);
	return (200);
}
