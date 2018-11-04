/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_pi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 11:28:57 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/26 12:04:22 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

char	*last_cmd;

int		exec_command(char **envp, char *cmd, char **argv)
{
	pid_t		pid;

	(void)envp;
	ft_strdel(argv);
	argv[0] = ft_strdup(cmd);
	pid = fork();
	if (!pid)
	{
		execv(cmd, (char *const*)argv);
		exit(1);
	}
	if (pid > 0)
		wait4(0, NULL, 0, NULL);

	return (200);
}

t_lcmd_hash cmd_hash[4] =
{
	{ 3, "lls",		"/bin/ls",	&exec_command },
	{ 3, "lcd",		NULL,		&lcommand_cd  },
	{ 4, "lpwd",	"/bin/pwd",	&exec_command },
	{ 0, NULL, NULL, NULL }
};

int		lcommand_handler(int socket, char *cmd, char **env)
{
	int		len;
	int		ret;
	char	*trim;
	char	**argv;
	int		i;

	trim = ft_strtrim(cmd);
	argv = ft_strsplit(trim, ' ');
	ft_strdel(&trim);

	last_cmd ? ft_strdel(&last_cmd) : 0;
	last_cmd = ft_strdup(argv[0]);

	len = ft_strlen(argv[0]);
	i = 0;
	while (cmd_hash[i].cmd_len)
	{
		if (cmd_hash[i].cmd_len == len && !ft_strcmp(cmd_hash[i].cmd, argv[0]))
		{
			ret = cmd_hash[i].exec(env, cmd_hash[i].bin, argv);
			break;
		}
		i++;
	}
	if (len == 3 && !ft_strcmp("put", argv[0]))
		ret = lcommand_put(socket, argv);
	else if (!cmd_hash[i].cmd_len)
		ret = 0;
	ft_tabdel(&argv);
	return (ret);
}
