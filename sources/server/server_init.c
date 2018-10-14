/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 16:25:13 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/12 18:06:17 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*g_root;
char	*g_home;
char	*g_out;

static void	free_path(char **home, char **path)
{
	ft_strdel(home);
	ft_strdel(path);
}

static void	error_init_server(char **home, char **path, int exit_val)
{
	printf("Error initializing server. Exiting...\n");
	free_path(home, path);
	exit (exit_val);
}

static int	init_home(char **home, char **path)
{
	DIR 	*dir;

	g_root = ft_strdup(*path);
	g_home = ft_strjoin(*path, "/home");
	g_out = ft_strjoin(*path, "/.out_file");

	if (!(dir = opendir(g_home)))
	{
		printf("Creating HOME directory...\n");

		if (mkdir(g_home, 0755) == -1)
			error_init_server(home, path, -1);
		if (chdir(g_home) == -1)
			error_init_server(home, path, -2);

		free_path(home, path);
		return (1);
	}
	if (chdir(g_home) == -1)
		error_init_server(home, path, -2);

	free_path(home, path);
	closedir(dir);
	return (0);
}

int			init_server(char **envp)
{
	char	*home;
	char	*path;
	DIR 	*dir;

	home = ft_strdup(*ft_tabstr(envp, "HOME=") + 5);
	path = ft_strjoin(home, "/.server");

	if (!(dir = opendir(path)))
	{
		printf("Creating SERVER directory...\n");
		if (mkdir(path, 0755) == -1)
			error_init_server(&home, &path, -1);

		return (init_home(&home, &path));
	}

	closedir(dir);
	return (init_home(&home, &path));
}
