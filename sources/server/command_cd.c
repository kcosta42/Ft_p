/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 17:29:26 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/14 18:52:42 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*g_root;
char	*g_home;

int			isvalid_path(char *path, char *arg)
{
	struct stat	st_stat;
	char		*tmp;
	char		*absolute_path;
	int			ret;
	int			fd;

	ret = 0;
	if (!ft_strcmp(arg, "-"))
		return (ret);
	if (*arg != '/')
	{
		tmp = ft_strjoin(path, "/");
		absolute_path = ft_strjoin(tmp, arg);
		ft_strdel(&tmp);
	}
	else
		absolute_path = ft_strdup(arg);
	fd = open(absolute_path, O_RDONLY);
	if (stat(absolute_path, &st_stat) == -1)
		ret = printf("cd: No such file or directory: %s\n", arg);
	else if (!S_ISDIR(st_stat.st_mode))
		ret = printf("cd: Not a directory: %s\n", arg);
	else if (access(absolute_path, X_OK) == -1)
		ret = printf("cd: Permission denied: %s\n", arg);
	ft_strdel(&absolute_path);
	return (ret);
}

char		**ft_path_split(char *absolute_path)
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

static int		ft_cd_back(char **pwd)
{
	char		*path;
	size_t		size;

	if (ft_strlen(*pwd) == ft_strlen(g_root) && !ft_strcmp(*pwd, g_root))
		return (0);
	size = (ft_strrchr(*pwd, '/') - *pwd) ? ft_strrchr(*pwd, '/') - *pwd : 1;
	path = ft_strnew(size);
	path = ft_strncpy(path, *pwd, size);
	chdir(path);
	ft_strdel(pwd);
	*pwd = path;
	return (0);
}

char		*ft_get_path(char **path)
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

static int		ft_cd_forward(char *arg, char **pwd)
{
	char		*path;
	char		*tmp;

	if (arg[0] == '/')
	{
		path = ft_strdup(arg);
		ft_strdel(pwd);
		*pwd = path;
		return (chdir(*pwd));
	}
	if (ft_strcmp(*pwd, "/"))
		tmp = ft_strjoin(*pwd, "/");
	else
		tmp = ft_strdup(*pwd);
	path = ft_strjoin(tmp, arg);
	ft_strdel(&tmp);
	chdir(path);
	ft_strdel(pwd);
	*pwd = ft_get_path(&path);
	return (0);
}

static int		ft_cd_manage(char **path, char **pwd, char **old)
{
	size_t		index;

	index = 0;
	if (!ft_strcmp(path[index], "-"))
	{
		ft_printf("%s\n", *old);
		chdir(*old);
		ft_strswp(pwd, old);
		return (200);
	}
	ft_strdel(old);
	*old = ft_strdup(*pwd);
	while (index < ft_tablen(path))
	{
		if (ft_strlen(path[index]) == 2 && !ft_strcmp(path[index], ".."))
			ft_cd_back(pwd);
		else if (ft_strlen(path[index]) == 1 && !ft_strcmp(path[index], "."))
			;
		else
			ft_cd_forward(path[index], pwd);
		index++;
	}
	return (200);
}

int		command_cd(int client, char **argv)
{
	static char	*pwd = NULL;
	static char	*old_pwd = NULL;
	char		**path;

	send_data(client, NULL, 0);
	pwd = (!pwd) ? ft_strdup(g_home) : pwd;
	old_pwd = (!old_pwd) ? ft_strdup(g_home) : old_pwd;
	if (ft_tablen(argv) > 2)
	{
		printf("cd: String not in pwd: %s\n", argv[1]);
		return (501);
	}
	if (!argv[1] || (ft_strlen(argv[1]) == 1 && !ft_strcmp(argv[1], "~")))
	{
		if (chdir(g_home) == -1)
		{
			printf("cd: HOME not found.\n");
			return (501);
		}
	}
	if (argv[1][0] == '/')
	{
		char	*tmp;
		tmp = ft_strjoin(g_root, argv[1] + 1);
		ft_strdel(&(argv[1]));
		argv[1] = ft_strdup(tmp);
		ft_strdel(&tmp);
	}
	if (isvalid_path(pwd, argv[1]))
		return (501);
	path = ft_path_split(argv[1]);
	ft_cd_manage(path, &pwd, &old_pwd);

	ft_tabdel(&path);
	return (200);
}
