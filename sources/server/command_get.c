/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 16:23:58 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/18 23:12:29 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int		send_filename(int client, char *path)
{
	char	*filename;
	char	*tmp;

	tmp = ft_strrchr(path, '/');
	filename = tmp ? ft_strdup(tmp + 1) : ft_strdup(path);
	send_data(client, filename, ft_strlen(filename));

	ft_strdel(&filename);
	return (0);
}

int		command_get(int client, char *cmd, char **argv)
{
	int			fd;
	struct stat	st_stat;
	void		*ptr;

	(void)cmd;
	if (ft_tablen(argv) != 2)
	{
		send_data(client, "Usage: get <file>", ft_strlen("Usage: get <file>"));
		return (501);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1 && errno == EACCES)
	{
		send_data(client, "Permission denied.", ft_strlen("Permission denied."));
		return (501);
	}
	else if (fd == -1)
	{
		send_data(client, "File does not exists.", ft_strlen("File does not exists."));
		return (501);
	}
	else if (fstat(fd, &st_stat) < 0)
	{
		send_data(client, "Permission denied.", ft_strlen("Permission denied."));
		return (501);
	}
	else if (S_ISDIR(st_stat.st_mode))
	{
		send_data(client, "Usage: get <file>", ft_strlen("Usage: get <file>"));
		return (501);
	}

	ptr = mmap(0, st_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	send_data(client, ptr, st_stat.st_size);
	send_filename(client, argv[1]);

	munmap(ptr, st_stat.st_size);
	return (200);
}
