/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_pi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 19:54:47 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/15 12:00:33 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*g_out;

int		exec_command(int client, char *cmd, char **argv)
{
	pid_t		pid;
	int			fd;
	char		*data;
	size_t		data_length;

	ft_strdel(argv);
	argv[0] = ft_strdup(cmd);
	pid = fork();
	fd = open(g_out, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!pid)
	{
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		execv(cmd, (char *const*)argv);
		exit(1);
	}
	if (pid > 0)
		wait4(0, NULL, 0, NULL);

	data_length = read_file(fd, &data);
	close(fd);

	send_data(client, data, (data_length == (size_t)-1) ? 0 : data_length);
	ft_strdel(&data);
	return (200);
}

int		command_pwd(int client, char *cmd, char **argv)
{
	pid_t		pid;
	int			fd;
	char		*data;
	size_t		data_length;

	ft_strdel(argv);
	argv[0] = ft_strdup(cmd);
	pid = fork();
	fd = open(g_out, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!pid)
	{
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		execv(cmd, (char *const*)argv);
		exit(1);
	}
	if (pid > 0)
		wait4(0, NULL, 0, NULL);

	data_length = read_file(fd, &data);
	close(fd);

	if (data_length == (size_t)-1)
		send_data(client, data, 0);
	else
	{
		char	*tmp;
		tmp = ft_strdup(data + ft_strlen(g_root));
		ft_strdel(&data);
		data = ft_strdup(*tmp ? tmp : "/");
		send_data(client, data, ft_strlen(data));
		ft_strdel(&tmp);
	}
	ft_strdel(&data);
	return (200);
}

int		command_quit(int client, char *cmd, char **argv)
{
	(void)cmd;
	(void)argv;
	send_data(client, "QUIT: 221 SUCCESS", sizeof("QUIT: 221 SUCCESS"));
	return (221);
}

t_cmd_hash cmd_hash[9] =
{
	{ 2, "ls",		"/bin/ls",		&exec_command },
	{ 2, "cp",		"/bin/cp",		&exec_command },
	{ 2, "mv",		"/bin/mv",		&exec_command },
	{ 2, "cd",		NULL,			&command_cd   },
	{ 3, "pwd",		"/bin/pwd",		&command_pwd  },
	{ 5, "mkdir",	"/bin/mkdir",	&exec_command },
	{ 5, "rmdir",	"/bin/rmdir",	&exec_command },
	{ 6, "quit",	NULL,			&command_quit },
	{ 0, NULL, NULL, NULL }
};

void	replace_path(char ***argv)
{
	char	*tmp;
	int		i;

	i = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i][0] == '/')
		{
			tmp = ft_strjoin(g_root, (*argv)[i] + 1);

			ft_strdel(&((*argv)[i]));
			(*argv)[i] = ft_strdup(tmp);
			ft_strdel(&tmp);
		}
		i++;
	}
}

int		command_handler(int client, char *cmd)
{
	int		len;
	int		ret;
	char	**argv;
	int		i;

	argv = ft_strsplit(ft_strtrim(cmd), ' ');
	len = ft_strlen(argv[0]);
	i = 0;
	while (cmd_hash[i].cmd_len)
	{
		if (cmd_hash[i].cmd_len == len && !ft_strcmp(cmd_hash[i].cmd, argv[0]))
		{
			replace_path(&argv);
			ret = cmd_hash[i].exec(client, cmd_hash[i].bin, argv);
			break;
		}
		i++;
	}
	if (!cmd_hash[i].cmd_len)
	{
		send_data(client, "COMMAND NOT FOUND: 500 ERROR", ft_strlen("COMMAND NOT FOUND: 500 ERROR"));
		ret = 500;
	}
	ft_tabdel(&argv);
	return (ret);
}
