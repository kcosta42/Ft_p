/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 16:26:27 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/15 12:41:06 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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
