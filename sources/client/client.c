/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:46:44 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/12 17:11:02 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	usage(char *exec)
{
	printf("Usage: %s <addr> <port>\n", exec);
	exit(-1);
}

int		create_client(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (!proto)
		exit(-1);

	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)))
	{
		printf("Connect error\n");
		exit(-2);
	}
	return (sock);
}

int		main(int ac, char **av)
{
	int		port;
	int		socket;
	int		r;
	char	buffer[1024];
	size_t	data_len;
	char	*data;

	if (ac != 3)
		usage(av[0]);

	port = ft_atoi(av[2]);
	socket = create_client(av[1], port);

	while ((r = read(STDIN_FILENO, buffer, 1024)) > 0)
	{
		// Send Command
		buffer[r - 1] = 0;
		if(!*buffer)
			continue ;
		write(socket, buffer, ft_strlen(buffer));

		// Received Result
		ft_memset(buffer, 0, 1024);

		if (recv(socket, &data_len, sizeof(data_len), 0) == -1)
		{
			printf("Error while receiving data length.\n");
			continue ;
		}

		if (!data_len)
			continue ;
		data = ft_strnew(data_len);
		if ((r = recv(socket, data, data_len, 0)) == -1)
		{
			printf("Error while receiving data.\n");
			continue ;
		}
		write(1, data, data_len);
		printf("\n[received %d bytes]\n", r);
		ft_strdel(&data);
	}
	close(socket);
	return (0);
}
