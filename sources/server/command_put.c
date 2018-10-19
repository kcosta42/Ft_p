/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_put.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 22:46:43 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/19 16:39:56 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int		command_put(int client, char *cmd, char **argv)
{
	int		fd;
	void	*data;
	size_t	data_size;

	(void)cmd;

	if (recv(client, &data_size, sizeof(data_size), 0) == -1)
		return (send_data(client, "Error receiving file.", ft_strlen("Error receiving file."), 501));

	printf("%zu\n", data_size);
	if (!data_size)
		return (0);

	data = ft_memalloc(data_size);

	if (recv(client, data, data_size, 0) == -1)
		return (send_data(client, "Error receiving file.", ft_strlen("Error receiving file."), 501));

	printf("%s\n", (char*)data);
	fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	write(fd, data, data_size);
	close(fd);

	ft_memdel(&data);
	return (send_data(client, NULL, 0, 200));
}
