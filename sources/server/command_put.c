/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_put.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 22:46:43 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/06 16:38:32 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int		putfile(int client, size_t data_size, char *filename)
{
	size_t	len;
	void	*data;
	int		fd;
	size_t	offset;
	size_t	size;

	len = ft_strlen("Error receiving file.");
	data = ft_memalloc(data_size);
	offset = 0;
	while (data_size - offset)
	{
		size = data_size - offset > 4096 ? 4096 : data_size - offset;
		if (recv(client, data + offset, size, 0) == -1)
			return (send_data(client, "Error receiving file.", len, 501));
		offset = (offset + 4096 < data_size) ? offset + 4096 : data_size;
	}
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	write(fd, data, data_size);
	close(fd);
	ft_memdel(&data);
	return (0);
}

int		command_put(int client, char *cmd, char **argv)
{
	size_t	data_size;
	size_t	len;

	(void)cmd;
	len = ft_strlen("Error receiving file.");
	send(client, &client, sizeof(client), 0);
	if (recv(client, &data_size, sizeof(size_t), 0) == -1)
		return (send_data(client, "Error receiving file.", len, 501));
	putfile(client, data_size, argv[1]);
	return (send_data(client, NULL, 0, 200));
}
