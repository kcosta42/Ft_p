/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_dtp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 16:51:34 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 13:02:16 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

char	*g_last_cmd;

int		create_file(int socket, char *data, size_t data_size)
{
	int		reply;
	char	*filename;
	size_t	size;
	int		fd;

	recv(socket, &reply, sizeof(int), 0);
	size = 0;
	recv(socket, &size, sizeof(size_t), 0);
	if (!size)
		return (-1);
	filename = ft_strnew(size);
	if (recv(socket, filename, size, 0) == -1)
	{
		ft_strdel(&filename);
		return (-1);
	}
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	write(fd, data, data_size);
	close(fd);
	ft_strdel(&filename);
	return (0);
}

char	*reply_msg(int reply)
{
	if (reply == 200)
		return (MSG_200);
	if (reply == 221)
		return (MSG_221);
	if (reply == 500)
		return (MSG_500);
	if (reply == 501)
		return (MSG_501);
	return (MSG_DEFAULT);
}

int		data_handler(int socket, char *data, size_t data_size, int reply)
{
	if (reply == 200 && ft_strlen(g_last_cmd) == 3
		&& !ft_strcmp(g_last_cmd, "get"))
		create_file(socket, data, data_size);
	else
		write(1, data, data_size);
	printf("\nReply: %d %s [received %zu bytes]\n",
		reply, reply_msg(reply), data_size);
	return (0);
}

int		receive_data(int socket)
{
	int		reply;
	size_t	data_size;
	char	*data;
	int		ret;

	recv(socket, &reply, sizeof(int), 0);
	if (recv(socket, &data_size, sizeof(data_size), 0) == -1)
		printf("Error while receiving data size.\n");
	if (!data_size)
		return (data_handler(socket, NULL, 0, reply));
	data = ft_strnew(data_size);
	ret = recv(socket, data, data_size, 0);
	if (ret == -1)
		printf("Error while receiving data.\n");
	data_handler(socket, data, data_size, reply);
	ft_strdel(&data);
	if (reply == 221)
		exit(0);
	return (ret);
}

int		send_command(int socket, char *buffer)
{
	send(socket, buffer, ft_strlen(buffer), 0);
	receive_data(socket);
	return (0);
}
