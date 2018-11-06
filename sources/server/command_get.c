/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 16:23:58 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/06 16:04:46 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	send_err_msg(int client, char *msg)
{
	return (send_data(client, msg, ft_strlen(msg), 501));
}

static int	send_filename(int client, char *path)
{
	char	*filename;
	char	*tmp;

	tmp = ft_strrchr(path, '/');
	filename = tmp ? ft_strdup(tmp + 1) : ft_strdup(path);
	send_data(client, filename, ft_strlen(filename), 200);
	ft_strdel(&filename);
	return (200);
}

int			command_get(int client, char *cmd, char **argv)
{
	int			fd;
	struct stat	st_stat;
	char		*ptr;

	(void)cmd;
	if (ft_tablen(argv) != 2)
		return (send_err_msg(client, "Usage: get <file>"));
	fd = open(argv[1], O_RDONLY);
	if (fd == -1 && errno == EACCES)
		return (send_err_msg(client, "Permission denied."));
	else if (fd == -1)
		return (send_err_msg(client, "File does not exists."));
	else if (fstat(fd, &st_stat) < 0)
		return (send_err_msg(client, "Permission denied."));
	else if (S_ISDIR(st_stat.st_mode))
		return (send_err_msg(client, "Usage: get <file>"));
	ptr = ft_strnew(st_stat.st_size);
	read(fd, ptr, st_stat.st_size);
	close(fd);
	send_data(client, ptr, st_stat.st_size, 200);
	send_filename(client, argv[1]);
	ft_strdel(&ptr);
	return (200);
}
