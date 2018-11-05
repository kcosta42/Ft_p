/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_pi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 19:54:47 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 16:49:59 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*g_out;

t_cmd_hash g_cmd_hash[12] =
{
	{ 2, "ls", "/bin/ls", &exec_command },
	{ 2, "cp", "/bin/cp", &exec_command },
	{ 2, "mv", "/bin/mv", &exec_command },
	{ 2, "cd", NULL, &command_cd },
	{ 3, "get", NULL, &command_get },
	{ 3, "put", NULL, &command_put },
	{ 3, "del", "/bin/rm", &command_del },
	{ 3, "pwd", "/bin/pwd", &command_pwd },
	{ 4, "quit", NULL, &command_quit },
	{ 5, "mkdir", "/bin/mkdir", &exec_command },
	{ 5, "rmdir", "/bin/rmdir", &exec_command },
	{ 0, NULL, NULL, NULL }
};

int			exec_command(int client, char *cmd, char **argv)
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

static int	send_err_msg(int client, char *cmd)
{
	char	*msg;
	int		ret;

	msg = ft_strjoin(cmd, ": No such file or directory.");
	send_data(client, msg, ft_strlen(msg), 200);
	ret = 200;
	ft_strdel(&msg);
	return (ret);
}

int			command_handler_(int client, size_t len, char **argv)
{
	int		i;
	int		ret;

	i = -1;
	while (g_cmd_hash[++i].cmd_len)
	{
		if (g_cmd_hash[i].cmd_len == len
			&& !ft_strcmp(g_cmd_hash[i].cmd, argv[0]))
		{
			if (replace_path(&argv) && i != 3)
			{
				ret = send_err_msg(client, g_cmd_hash[i].cmd);
				break ;
			}
			ret = g_cmd_hash[i].exec(client, g_cmd_hash[i].bin, argv);
			break ;
		}
	}
	if (!g_cmd_hash[i].cmd_len)
	{
		send_data(client, "Command not found.",
			ft_strlen("Command not found."), 500);
		ret = 500;
	}
	return (ret);
}

int			command_handler(int client, char *cmd)
{
	int		len;
	int		ret;
	char	**argv;
	char	*trim;

	trim = ft_strtrim(cmd);
	argv = ft_strsplit(trim, ' ');
	ft_strdel(&trim);
	len = ft_strlen(argv[0]);
	ret = command_handler_(client, len, argv);
	ft_tabdel(&argv);
	return (ret);
}
