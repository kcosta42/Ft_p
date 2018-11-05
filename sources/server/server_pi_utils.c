/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_pi_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 15:32:43 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 16:39:09 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int		command_quit(int client, char *cmd, char **argv)
{
	(void)cmd;
	(void)argv;
	return (send_data(client, "Exiting...", ft_strlen("Exiting..."), 221));
}

int		check_path(char *path)
{
	char	**cwd;
	char	*tmp;
	char	buffer[1024];
	size_t	index;

	tmp = path;
	getcwd(buffer, 1024);
	cwd = ft_strsplit(buffer, '/');
	index = ft_tablen(cwd);
	while (tmp)
	{
		if (ft_strlen(tmp) > 2 && !ft_strncmp(tmp, "..", 2))
			index--;
		else
			index++;
		tmp = ft_strchr(tmp, '/');
		tmp = tmp ? tmp + 1 : NULL;
	}
	ft_tabdel(&cwd);
	return (index < 5);
}

int		replace_path(char ***argv)
{
	char	*tmp;
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i][0] == '/')
		{
			tmp = ft_strjoin(g_root, (*argv)[i] + 1);
			ft_strdel(&((*argv)[i]));
			(*argv)[i] = ft_strdup(tmp);
			ft_strdel(&tmp);
		}
		if (ft_strstr((*argv)[i], ".."))
			ret = check_path((*argv)[i]);
		i++;
	}
	return (ret);
}
