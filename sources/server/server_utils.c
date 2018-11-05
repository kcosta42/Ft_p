/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 15:22:29 by kcosta            #+#    #+#             */
/*   Updated: 2018/11/05 15:55:41 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int			send_data(int client, const void *data, size_t size, int reply)
{
	int		ret;

	send(client, &reply, sizeof(int), 0);
	ret = send(client, &size, sizeof(size_t), 0);
	if (ret == -1)
		printf("Failed to deliver data size.\n");
	if (!size)
		return (0);
	ret = send(client, data, size, 0);
	if (ret == -1)
		printf("Failed to deliver data.\n");
	return (reply);
}
