/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcommand_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 12:31:37 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/16 19:20:42 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

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
	if (fd == -1 && errno == EACCES)
		ret = printf("cd: Permission denied: %s\n", arg);
	else if (fd == -1)
		ret = printf("cd: No such file or directory: %s\n", arg);
	else if (fstat(fd, &st_stat) == -1)
		ret = printf("cd: Permission denied: %s\n", arg);
	else if (!S_ISDIR(st_stat.st_mode))
		ret = printf("cd: Not a directory: %s\n", arg);
	ft_strdel(&absolute_path);
	return (ret);
}

static int		lcd_home(char *home, char **pwd, char **old_pwd)
{
	if (chdir(home) == -1)
		return (printf("cd: HOME not found.\n"));

	ft_strdel(old_pwd);
	*old_pwd = ft_strdup(*pwd);
	ft_strdel(pwd);
	*pwd = ft_strdup(home);

	return (0);
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

static int		lcd_back(char **pwd)
{
	char		*path;
	size_t		size;

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

static int		lcd_forward(char *arg, char **pwd)
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

static int		lcd_manage(char **path, char **pwd, char **old)
{
	size_t		index;

	index = 0;
	if (!ft_strcmp(path[index], "-"))
	{
		ft_printf("%s\n", *old);
		chdir(*old);
		ft_strswp(pwd, old);
		return (0);
	}
	ft_strdel(old);
	*old = ft_strdup(*pwd);
	while (index < ft_tablen(path))
	{
		if (!ft_strcmp(path[index], ".."))
			lcd_back(pwd);
		else if (!ft_strcmp(path[index], "."))
			;
		else
			lcd_forward(path[index], pwd);
		index++;
	}
	return (0);
}

int		lcommand_cd(char **env, char *cmd, char **argv)
{
	static char	*home = NULL;
	static char	*pwd = NULL;
	static char	*old_pwd = NULL;
	char		**path;

	(void)cmd;
	pwd 	= (!pwd) 	 ? ft_strdup(*ft_tabstr(env, "PWD=") + 4) : pwd;
	old_pwd = (!old_pwd) ? ft_strdup(*ft_tabstr(env, "HOME=") + 5) : old_pwd;
	home 	= (!home) 	 ? ft_strdup(*ft_tabstr(env, "HOME=") + 5) : old_pwd;

	if (ft_tablen(argv) > 2)
		return (printf("cd: String not in pwd.\n"));

	if (!argv[1] || !ft_strcmp(argv[1], "~"))
		return (lcd_home(home, &pwd, &old_pwd));

	if (isvalid_path(pwd, argv[1]))
		return (1);

	path = path_split(argv[1]);
	lcd_manage(path, &pwd, &old_pwd);

	ft_tabdel(&path);
	return (200);
}
