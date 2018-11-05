/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 14:47:30 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 15:01:24 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	send_err_msg(int client, char *str, char *arg, int errnum)
{
	char		*msg;

	msg = ft_strjoin(str, arg);
	send_data(client, msg, ft_strlen(msg), 501);
	ft_strdel(&msg);
	return (errnum);
}

int			isvalid_path_(int fd, int client, char *arg)
{
	struct stat	st_stat;
	int			ret;

	ret = 0;
	if (fd == -1 && errno == EACCES)
		ret = send_err_msg(client, "cd: Permission denied: ", arg, 1);
	else if (fd == -1)
		ret = send_err_msg(client, "cd: No such file or directory: ", arg, 2);
	else if (fstat(fd, &st_stat) == -1)
		ret = send_err_msg(client, "cd: Permission denied: ", arg, 1);
	else if (!S_ISDIR(st_stat.st_mode))
		ret = send_err_msg(client, "cd: Not a directory: ", arg, 3);
	return (ret);
}

int			isvalid_path(int client, char *path, char *arg)
{
	char	*tmp;
	char	*absolute_path;
	int		ret;
	int		fd;

	if (!ft_strcmp(arg, "-"))
		return (0);
	if (*arg != '/')
	{
		tmp = ft_strjoin(path, "/");
		absolute_path = ft_strjoin(tmp, arg);
		ft_strdel(&tmp);
	}
	else
		absolute_path = ft_strdup(arg);
	fd = open(absolute_path, O_RDONLY);
	ret = isvalid_path_(fd, client, arg);
	close(fd);
	ft_strdel(&absolute_path);
	return (ret);
}

char		**path_split(char *absolute_path)
{
	char	**path;
	char	*tmp;

	if (*absolute_path == '/')
	{
		tmp = ft_strjoin(" ", absolute_path);
		path = ft_strsplit(tmp, '/');
		path[0][0] = '/';
		ft_strdel(&tmp);
	}
	else
		path = ft_strsplit(absolute_path, '/');
	return (path);
}

char		*get_path(char **path)
{
	char	*absolute_path;
	size_t	index;

	index = ft_strlen(*path) - 1;
	while ((*path)[index] == '/')
		index--;
	absolute_path = ft_strnew(index + 1);
	absolute_path = ft_strncpy(absolute_path, *path, index + 1);
	ft_strdel(path);
	return (absolute_path);
}
