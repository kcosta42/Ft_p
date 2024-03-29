/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:46:44 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 15:55:39 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	usage(char *exec)
{
	printf("Usage: %s <port>\n", exec);
	exit(-1);
}

static int	create_server(int port)
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
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)))
	{
		printf("Bind error\n");
		exit(-2);
	}
	if (listen(sock, 42))
	{
		printf("Serveur failed to listen\n");
		exit(-3);
	}
	return (sock);
}

int			manage_client(int client)
{
	int		r;
	int		ret;
	char	buffer[1024];

	if ((r = recv(client, buffer, 1023, 0)) > 0)
	{
		buffer[r] = 0;
		printf("[received %d bytes] -- %s\n", r, buffer);
		ret = command_handler(client, buffer);
		return (ret);
	}
	return (221);
}

static int	accept_client(int sock)
{
	int					cs;
	struct sockaddr_in	csin;
	socklen_t			cslen;
	pid_t				pid;

	cs = accept(sock, (struct sockaddr *)&csin, &cslen);
	pid = fork();
	if (!pid)
	{
		while (manage_client(cs) != 221)
			;
		close(cs);
		exit(1);
	}
	if (pid > 0)
		wait4(-1, NULL, WNOHANG, NULL);
	return (0);
}

int			main(int ac, char **av, char **e)
{
	int		port;
	int		sock;

	if (ac != 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
	sock = create_server(port);
	init_server(e);
	while (42)
		accept_client(sock);
	close(sock);
	return (0);
}
