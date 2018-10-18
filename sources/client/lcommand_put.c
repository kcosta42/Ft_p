/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcommand_put.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 22:52:13 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/19 00:37:01 by kcosta           ###   ########.fr       */
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

int		send_data(int socket, char *ptr, char *path)
{
	char	*tmp;
	char	*tmp2;
	char	*filename;
	char	*data;

	filename = get_filename(path);

	tmp = ft_strjoin("put ", filename);
	tmp2 = ft_strjoin(" ", ptr);
	data = ft_strjoin(tmp, tmp2);

	send_command(socket, data);

	ft_strdel(&tmp);
	ft_strdel(&tmp2);
	ft_strdel(&filename);
	ft_strdel(&data);
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

	send_data(socket, ptr, argv[1]);

	munmap(ptr, st_stat.st_size);
	return (200);
}
