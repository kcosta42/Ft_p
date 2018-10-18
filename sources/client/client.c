/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:46:44 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/18 22:45:05 by kcosta           ###   ########.fr       */
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

int		main(int ac, char **av, char **env)
{
	int		port;
	int		socket;
	int		r;
	char	buffer[1024];

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

		if (!lcommand_handler(socket, buffer, env))
			send_command(socket, buffer);

		ft_memset(buffer, 0, 1024);
	}
	close(socket);
	return (0);
}
