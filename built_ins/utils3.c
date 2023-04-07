/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: busmanov <busmanov@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 18:43:19 by busmanov          #+#    #+#             */
/*   Updated: 2023/04/07 18:43:20 by busmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void		*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size);
	ft_memcpy(new_ptr, ptr, size);
	return (new_ptr);
}

void	*ft_memcpy(void *to, const void *from, size_t n)
{
	unsigned char	*pto;
	unsigned char	*pfrom;
	size_t			x;

	if (to == NULL && from == NULL)
		return (NULL);
	pto = (unsigned char *)to;
	pfrom = (unsigned char *)from;
	x = 0;
	while (x < n)
	{
		pto[x] = pfrom[x];
		x++;
	}
	return (pto);
}