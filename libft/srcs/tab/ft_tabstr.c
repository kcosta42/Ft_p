/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 12:36:12 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/15 12:37:45 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tab.h"
#include "ft_string.h"

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
