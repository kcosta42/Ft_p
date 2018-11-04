/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcommand_put.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 22:52:13 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/26 11:40:03 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

char	*get_filename(char *path)
{
	char	*filename;
	char	*tmp;

	tmp = ft_strrchr(path, '/');
	filename = tmp ? ft_strdup(tmp + 1) : ft_strdup(path);

	return (filename);
}

int		send_data(int socket, char *path, char *ptr, size_t size)
{
	char	*cmd;
	char	*filename;
	int		ready;

	filename = get_filename(path);

	cmd = ft_strjoin("put ", filename);

	send(socket, cmd, ft_strlen(cmd), 0);
	recv(socket, &ready, sizeof(int), 0);

	if (send(socket, &size, sizeof(size_t), 0) == -1)
	{
		ft_strdel(&cmd);
		ft_strdel(&filename);
		return (receive_data(socket));
	}
	if (size)
		send(socket, ptr, size, 0);

	receive_data(socket);

	ft_strdel(&cmd);
	ft_strdel(&filename);
	return (0);
}

int		lcommand_put(int socket, char **argv)
{
	int			fd;
	struct stat	st_stat;
	void		*ptr;

	if (ft_tablen(argv) != 2)
		return (printf("Usage: put <file>\n"));

	fd = open(argv[1], O_RDONLY);
	if (fd == -1 && errno == EACCES)
		return (printf("Permission denied.\n"));
	else if (fd == -1)
		return (printf("File does not exists.\n"));
	else if (fstat(fd, &st_stat) < 0)
		return (printf("Permission denied.\n"));
	else if (S_ISDIR(st_stat.st_mode))
		return (printf("Usage: put <file>\n"));

	ptr = mmap(0, st_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);

	send_data(socket, argv[1], ptr, st_stat.st_size);

	munmap(ptr, st_stat.st_size);
	return (200);
}
