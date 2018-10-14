/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_pi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 19:54:47 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/12 18:58:22 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*g_out;

int		command_pwd(int client, char **argv)
{
	pid_t		pid;
	int			fd;
	char		*data;
	size_t		data_length;

	ft_strdel(argv);
	argv[0] = ft_strdup("/bin/pwd");
	pid = fork();
	fd = open(g_out, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!pid)
	{
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		execv("/bin/pwd", (char *const*)argv);
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

int		command_ls(int client, char **argv)
{
	pid_t		pid;
	int			fd;
	char		*data;
	size_t		data_length;

	ft_strdel(argv);
	argv[0] = ft_strdup("/bin/ls");
	pid = fork();
	fd = open(g_out, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!pid)
	{
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		execv("/bin/ls", (char *const*)argv);
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

int		command_quit(int client)
{
	send_data(client, "221 SUCCESS", sizeof("221 SUCCESS"));
	return (221);
}

int		command_handler(int client, char *cmd)
{
	int		len;
	int		ret;
	char	**argv;

	argv = ft_strsplit(ft_strtrim(cmd), ' ');
	len = ft_strlen(argv[0]);
	if (len == 2 && !ft_strcmp("ls", argv[0]))
		ret = command_ls(client, argv);
	else if (len == 3 && !ft_strcmp("pwd", argv[0]))
		ret = command_pwd(client, argv);
	else if (len == 2 && !ft_strcmp("cd", argv[0]))
		ret = command_cd(client, argv);
	// else if (len == 4 && !ft_strcmp("mkdir", argv[0]))
	// 	ret = command_mkdir(client);
	// else if (len == 4 && !ft_strcmp("rmdir", argv[0]))
	// 	ret = command_rmdir(client);
	// else if (len == 4 && !ft_strcmp("rm", argv[0]))
	// 	ret = command_rm(client);
	else if (len == 4 && !ft_strcmp("quit", argv[0]))
		ret = command_quit(client);
	else
	{
		printf("COMMAND NOT FOUND\n");
		send_data(client, "500 ERROR", sizeof("500 ERROR"));
		ret = 500;
	}
	ft_tabdel(&argv);
	return (ret);
}
