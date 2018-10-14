/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 16:26:27 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/12 20:28:34 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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

void	ft_tabdel(char ***tab)
{
	int		i;

	i = 0;
	while ((*tab)[i])
	{
		ft_strdel(&((*tab)[i]));
		i++;
	}
	ft_strdel((char**)tab);
}

size_t	ft_tablen(char **tab)
{
	size_t	size;
	size_t	index;

	size = 0;
	index = 0;
	while (tab[index])
	{
		if (*tab[index])
			size++;
		index++;
	}
	return (size);
}

size_t	read_file(const int fd, char **line)
{
	char	buff[1024];
	char	*dup;
	char	*tmp;
	int		ret;
	size_t	len;

	dup = ft_strnew(0);
	len = 0;
	while ((ret = read(fd, buff, 1023)) > 0)
	{
		len += ret;
		buff[ret] = 0;
		tmp = dup;
		dup = ft_strjoin(dup, buff);
		ft_strdel(&tmp);
	}
	*line = ft_strdup(dup);
	ft_strdel(&dup);

	(*line)[len - 1] = 0;
	return (len - 1);
}
