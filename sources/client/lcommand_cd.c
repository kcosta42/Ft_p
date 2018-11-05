/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcommand_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 12:31:37 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 13:48:55 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static int		lcd_home(char *home, char **pwd, char **old_pwd)
{
	if (chdir(home) == -1)
		return (printf("cd: HOME not found.\n"));
	ft_strdel(old_pwd);
	*old_pwd = ft_strdup(*pwd);
	ft_strdel(pwd);
	*pwd = ft_strdup(home);
	return (1);
}

static int		lcd_back(char **pwd)
{
	char		*path;
	size_t		size;

	if (!*pwd || !pwd)
		return (1);
	size = (ft_strrchr(*pwd, '/') - *pwd) ? ft_strrchr(*pwd, '/') - *pwd : 1;
	if (size > 1024)
		return (1);
	path = ft_strnew(size);
	path = ft_strncpy(path, *pwd, size);
	chdir(path);
	ft_strdel(pwd);
	*pwd = path;
	return (1);
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

int				lcommand_cd(char **env, char *cmd, char **argv)
{
	static char	*home = NULL;
	static char	*pwd = NULL;
	static char	*old_pwd = NULL;
	char		**path;

	(void)cmd;
	pwd = (!pwd) ? ft_strdup(*ft_tabstr(env, "PWD=") + 4) : pwd;
	old_pwd = (!old_pwd) ? ft_strdup(*ft_tabstr(env, "HOME=") + 5) : old_pwd;
	home = (!home) ? ft_strdup(*ft_tabstr(env, "HOME=") + 5) : home;
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
