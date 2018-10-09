/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 11:46:44 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/09 16:09:03 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

pid_t	g_process;

char	**ft_tabstr(char **tab, char *entry)
{
	while (*tab)
	{
		if (!ft_strncmp(*tab, entry, ft_strlen(entry)))
			return (tab);
		tab++;
	}
	return (NULL);
}

void	usage(char *exec)
{
	printf("Usage: %s <port>\n", exec);
	exit(-1);
}

int		create_server(int port)
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

int		init_server(char **envp)
{
	char	*home;
	char	*path;
	DIR 	*dir;

	home = ft_strdup(*ft_tabstr(envp, "HOME=") + 5);
	path = ft_strjoin(home, "/.server");

	if (! (dir = opendir(path)))
	{
		printf("Creating server directory...\n");
		if (mkdir(path, 0644))
		{
			printf("Error initializing server. Exiting...\n");
			closedir(dir);
			ft_strdel(&home);
			ft_strdel(&path);
			exit (-1);
		}
	}

	closedir(dir);
	ft_strdel(&home);
	ft_strdel(&path);
	return (0);
}

int manage_client(int client)
{
	int		r;
	char	buffer[1024];

	if ((r = read(client, buffer, 1023)) > 0)
	{
		buffer[r] = 0;
		printf("received %d bytes: [%s]\n", r, buffer);
		return(0);
	}
	return(-1);
}

int accept_client(int sock)
{
	int					cs;
	struct sockaddr_in	csin;
	socklen_t			cslen;

	cs = accept(sock, (struct sockaddr *)&csin, &cslen);
	g_process = fork();
	if (!g_process)
	{
		while (manage_client(cs) >= 0)
			;
		close(cs);
	}
	if (g_process > 0)
		wait4(-1, NULL, WNOHANG, NULL);
	if (!g_process)
		exit(1);
	return (0);
}

int		main(int ac, char **av, char **e)
{
	int		port;
	int		sock;

	if (ac != 2)
		usage(av[0]);

	port = ft_atoi(av[1]);
	sock = create_server(port);
	init_server(e);
	while (42)
	{
		g_process = 0;
		accept_client(sock);
	}
	close(sock);
	return (0);
}
