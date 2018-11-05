/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 17:29:26 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 14:50:25 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*g_root;
char	*g_home;

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
	size_t		len;

	len = ft_strlen("cd: HOME not found.");
	if (chdir(home) == -1)
		return (send_data(client, "cd: HOME not found.", len, 501));
	ft_strdel(old_pwd);
	*old_pwd = ft_strdup(*pwd);
	ft_strdel(pwd);
	*pwd = ft_strdup(home);
	return (send_data(client, NULL, 0, 200));
}

int				command_cd(int client, char *cmd, char **argv)
{
	static char	*pwd = NULL;
	static char	*old_pwd = NULL;
	char		**path;
	char		*msg;

	(void)cmd;
	pwd = (!pwd) ? ft_strdup(g_home) : pwd;
	old_pwd = (!old_pwd) ? ft_strdup(g_home) : old_pwd;
	if (ft_tablen(argv) > 2)
	{
		msg = ft_strjoin("cd: String not in pwd: ", argv[1]);
		send_data(client, msg, ft_strlen(msg), 501);
		ft_strdel(&msg);
		return (501);
	}
	if (!argv[1] || (ft_strlen(argv[1]) == 1 && !ft_strcmp(argv[1], "~")))
		return (cd_home(client, g_home, &pwd, &old_pwd));
	if (isvalid_path(client, pwd, argv[1]))
		return (501);
	path = path_split(argv[1]);
	cd_manage(path, &pwd, &old_pwd);
	ft_tabdel(&path);
	return (send_data(client, NULL, 0, 200));
}
