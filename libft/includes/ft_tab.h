/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tab.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 12:34:11 by kcosta            #+#    #+#             */
/*   Updated: 2018/10/15 12:38:40 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TAB_H
# define FT_TAB_H

# include <string.h>

void	ft_tabdel(char ***tab);
size_t	ft_tablen(char **tab);
char	**ft_tabdup(char **tab, char *entry);
char	**ft_tabstr(char **tab, char *entry);
int		ft_tabreplace(char ***tab, char *to_find, char *entry);

#endif
