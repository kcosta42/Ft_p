/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_pi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 19:54:47 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/30 15:06:05 by kcosta           ###   ########.fr       */
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

	data_length = ft_readfile(fd, &data);
	close(fd);

	send_data(client, data, (data_length == (size_t)-1) ? 0 : data_length, 200);
	ft_strdel(&data);
	return (200);
}

int		command_pwd(int client, char *cmd, char **argv)
{
	pid_t		pid;
	int			fd;
	char		*data;
	size_t		data_length;

	if (ft_tablen(argv) != 1)
		return (send_data(client, "pwd: too many arguments", ft_strlen("pwd: too many arguments"), 501));
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

	data_length = ft_readfile(fd, &data);
	close(fd);

	if (data_length == (size_t)-1)
		send_data(client, data, 0, 200);
	else
	{
		char	*tmp;
		tmp = ft_strdup(data + ft_strlen(g_root));
		ft_strdel(&data);
		data = ft_strdup(*tmp ? tmp : "/");
		send_data(client, data, ft_strlen(data), 200);
		ft_strdel(&tmp);
	}
	ft_strdel(&data);
	return (200);
}

int		command_del(int client, char *cmd, char **argv)
{
	pid_t		pid;
	int			fd;
	char		*data;
	size_t		data_length;

	if (ft_tablen(argv) > 2 || *argv[1] == '-')
		return (send_data(client, "Usage: del <file>", ft_strlen("Usage: del <file>"), 501));

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

	data_length = ft_readfile(fd, &data);
	close(fd);

	send_data(client, data, (data_length == (size_t)-1) ? 0 : data_length, 200);
	ft_strdel(&data);
	return (200);
}

int		command_quit(int client, char *cmd, char **argv)
{
	(void)cmd;
	(void)argv;
	return (send_data(client, "Exiting...", ft_strlen("Exiting..."), 221));
}

t_cmd_hash cmd_hash[12] =
{
	{ 2, "ls",		"/bin/ls",		&exec_command },
	{ 2, "cp",		"/bin/cp",		&exec_command },
	{ 2, "mv",		"/bin/mv",		&exec_command },
	{ 2, "cd",		NULL,			&command_cd   },
	{ 3, "get",		NULL,			&command_get  },
	{ 3, "put",		NULL,			&command_put  },
	{ 3, "del",		"/bin/rm",		&command_del  },
	{ 3, "pwd",		"/bin/pwd",		&command_pwd  },
	{ 4, "quit",	NULL,			&command_quit },
	{ 5, "mkdir",	"/bin/mkdir",	&exec_command },
	{ 5, "rmdir",	"/bin/rmdir",	&exec_command },
	{ 0, NULL, NULL, NULL }
};

int		check_path(char *path)
{
	char	**cwd;
	char	*tmp;
	char	buffer[1024];
	size_t	index;

	tmp = path;
	getcwd(buffer, 1024);
	cwd = ft_strsplit(buffer, '/');
	index = ft_tablen(cwd);
	while (tmp)
	{
		if (ft_strlen(tmp) > 2 && !ft_strncmp(tmp, "..", 2))
			index--;
		else
			index++;
		tmp = ft_strchr(tmp, '/');
		tmp = tmp ? tmp + 1 : NULL;
	}

	ft_tabdel(&cwd);
	return (index < 5);
}

int		replace_path(char ***argv)
{
	char	*tmp;
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i][0] == '/')
		{
			tmp = ft_strjoin(g_root, (*argv)[i] + 1);

			ft_strdel(&((*argv)[i]));
			(*argv)[i] = ft_strdup(tmp);
			ft_strdel(&tmp);
		}
		if (ft_strstr((*argv)[i], ".."))
			ret = check_path((*argv)[i]);
		i++;
	}
	return (ret);
}

int		command_handler(int client, char *cmd)
{
	int		len;
	int		ret;
	char	**argv;
	char	*trim;
	int		i;
	char	*msg;

	trim = ft_strtrim(cmd);
	argv = ft_strsplit(trim, ' ');
	ft_strdel(&trim);

	len = ft_strlen(argv[0]);

	i = 0;
	while (cmd_hash[i].cmd_len)
	{
		if (cmd_hash[i].cmd_len == len && !ft_strcmp(cmd_hash[i].cmd, argv[0]))
		{
			if (replace_path(&argv) && i != 3)
			{
				msg = ft_strjoin(cmd_hash[i].cmd, ": No such file or directory.");
				send_data(client, msg, ft_strlen(msg), 200);
				ret = 200;
				ft_strdel(&msg);
				break;
			}
			ret = cmd_hash[i].exec(client, cmd_hash[i].bin, argv);
			break;
		}
		i++;
	}
	if (!cmd_hash[i].cmd_len)
	{
		send_data(client, "Command not found.", ft_strlen("Command not found."), 500);
		ret = 500;
	}
	ft_tabdel(&argv);
	return (ret);
}
