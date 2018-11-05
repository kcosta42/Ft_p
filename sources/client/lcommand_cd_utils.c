/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcommand_cd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 13:46:11 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 14:34:56 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static int	isvalid_path_(int fd, char *arg)
{
	struct stat	st_stat;
	int			ret;

	ret = 0;
	if (fd == -1 && errno == EACCES)
		ret = printf("cd: Permission denied: %s\n", arg);
	else if (fd == -1)
		ret = printf("cd: No such file or directory: %s\n", arg);
	else if (fstat(fd, &st_stat) == -1)
		ret = printf("cd: Permission denied: %s\n", arg);
	else if (!S_ISDIR(st_stat.st_mode))
		ret = printf("cd: Not a directory: %s\n", arg);
	return (ret);
}

int			isvalid_path(char *path, char *arg)
{
	char		*tmp;
	char		*absolute_path;
	int			ret;
	int			fd;

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
	ret = isvalid_path_(fd, arg);
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
