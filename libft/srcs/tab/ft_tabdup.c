/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 12:37:08 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/15 12:38:34 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tab.h"
#include "ft_memory.h"
#include "ft_string.h"

char	**ft_tabdup(char **tab, char *entry)
{
	char	**new;
	size_t	size;
	size_t	i;
	size_t	j;

	i = -1;
	j = 0;
	size = ft_tablen(tab) + 1 + (entry != NULL);
	if (!(new = (char**)ft_memalloc(sizeof(char*) * size)))
		return (NULL);
	while (tab[++i])
	{
		if (*tab[i])
			new[j++] = ft_strdup(tab[i]);
		else if (entry)
		{
			new[j++] = ft_strdup(entry);
			ft_strdel(&entry);
		}
	}
	if (entry)
		new[j++] = ft_strdup(entry);
	return (new);
}
