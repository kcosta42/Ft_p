/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 12:35:43 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/15 12:38:51 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tab.h"

int		ft_tabreplace(char ***tab, char *to_find, char *entry)
{
	char	**tmp;
	char	**result;

	result = ft_tabstr(*tab, to_find);
	if (result)
		**result = 0;
	tmp = *tab;
	*tab = ft_tabdup(*tab, entry);
	ft_tabdel(&tmp);
	return (0);
}
