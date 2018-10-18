/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 17:29:26 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/16 14:03:18 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*g_root;
char	*g_home;

int			isvalid_path(int client, char *path, char *arg)
{
	struct stat	st_stat;
	char		*tmp;
	char		*absolute_path;
	int			ret;
	char		*msg;
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
	if (fd == -1 && errno == EACCES)
	{
		ret = 1;
		msg = ft_strjoin("cd: Permission denied: ", arg);
		send_data(client, msg, ft_strlen(msg));
		ft_strdel(&msg);
	}
	else if (fd == -1)
	{
		ret = 2;
		msg = ft_strjoin("cd: No such file or directory: ", arg);
		send_data(client, msg, ft_strlen(msg));
		ft_strdel(&msg);
	}
	else if (fstat(fd, &st_stat) == -1)
	{
		ret = 1;
		msg = ft_strjoin("cd: Permission denied: ", arg);
		send_data(client, msg, ft_strlen(msg));
		ft_strdel(&msg);
	}
	else if (!S_ISDIR(st_stat.st_mode))
	{
		ret = 3;
		msg = ft_strjoin("cd: Not a directory: ", arg);
		send_data(client, msg, ft_strlen(msg));
		ft_strdel(&msg);
	}
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

static int		cd_back(char **pwd)
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

static int		cd_forward(char *arg, char **pwd)
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
	*pwd = get_path(&path);
	return (0);
}

static int		cd_manage(char **path, char **pwd, char **old)
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
			cd_back(pwd);
		else if (ft_strlen(path[index]) == 1 && !ft_strcmp(path[index], "."))
			;
		else
			cd_forward(path[index], pwd);
		index++;
	}
	return (200);
}

static int		cd_home(int client, char *home, char **pwd, char **old_pwd)
{
	if (chdir(home) == -1)
	{
		send_data(client, "cd: HOME not found.", ft_strlen("cd: HOME not found."));
		return (501);
	}
	ft_strdel(old_pwd);
	*old_pwd = ft_strdup(*pwd);
	ft_strdel(pwd);
	*pwd = ft_strdup(home);
	send_data(client, NULL, 0);
	return(200);
}

int		command_cd(int client, char *cmd, char **argv)
{
	static char	*pwd = NULL;
	static char	*old_pwd = NULL;
	char		**path;
	char		*msg;

	(void)cmd;
	pwd 	= (!pwd) 	 ? ft_strdup(g_home) : pwd;
	old_pwd = (!old_pwd) ? ft_strdup(g_home) : old_pwd;
	if (ft_tablen(argv) > 2)
	{
		msg = ft_strjoin("cd: String not in pwd: ", argv[1]);
		send_data(client, msg, ft_strlen(msg));
		ft_strdel(&msg);
		return (501);
	}
	if (!argv[1] || (ft_strlen(argv[1]) == 1 && !ft_strcmp(argv[1], "~")))
		return (cd_home(client, g_home, &pwd, &old_pwd));
	if (isvalid_path(client, pwd, argv[1]))
		return (501);
	path = path_split(argv[1]);
	cd_manage(path, &pwd, &old_pwd);

	send_data(client, NULL, 0);
	ft_tabdel(&path);
	return (200);
}
