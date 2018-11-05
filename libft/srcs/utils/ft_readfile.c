/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readfile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 22:48:18 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 12:47:38 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_utils.h"
#include "ft_string.h"

size_t	ft_readfile(const int fd, char **buf)
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
	*buf = ft_strdup(dup);
	ft_strdel(&dup);
	(*buf)[len - 1] = 0;
	return (len - 1);
}
