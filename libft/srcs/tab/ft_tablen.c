/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tablen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 12:36:46 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/15 12:38:37 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tab.h"

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
