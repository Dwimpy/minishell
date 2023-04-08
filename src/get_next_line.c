/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 13:01:46 by tkilling          #+#    #+#             */
/*   Updated: 2023/01/06 14:16:27 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	ft_initialize(char **ptr, size_t *count, size_t *len);
static int	ft_if_not_x(int x, size_t *count, size_t *len);
static char	*ft_allocate(size_t *count, size_t len, char *ptr, char *buffer);

char	*get_next_line(int fd)
{
	size_t	count;
	size_t	len;
	char	buffer[BUFFER_SIZE];
	char	*ptr;
	int		x;

	if (ft_initialize(&ptr, &count, &len))
		return (NULL);
	x = read(fd, &buffer[count++], 1);
	if (x == -1 || x == 0)
		return (NULL);
	while (x > 0 && buffer[count - 1] != '\n' && buffer[count - 1] != '\0')
	{
		if (count == BUFFER_SIZE)
		{
			ptr = ft_allocate(&count, len, ptr, buffer);
			if (!ptr)
				return (NULL);
		}
		x = read(fd, &buffer[count++], 1);
		len++;
	}
	if (ft_if_not_x(x, &count, &len))
		return (NULL);
	return (ft_allocate(&count, len, ptr, buffer));
}

static char	*ft_allocate(size_t *count, size_t len, char *ptr, char *buffer)
{
	char	*new_str;

	if (len == 0)
		return (NULL);
	if (*count == 0)
		return (ptr);
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	new_str[len] = '\0';
	len = len - *count;
	while ((*count)-- != 0)
		new_str[len + *count] = buffer[*count];
	while (len-- != 0)
		new_str[len] = ptr[len];
	if (ptr)
		free(ptr);
	*count = 0;
	return (new_str);
}

static int	ft_if_not_x(int x, size_t *count, size_t *len)
{
	if (x == -1)
	{
		return (1);
	}
	if (x == 0)
	{
		(*count)--;
		(*len)--;
	}
	return (0);
}

static int	ft_initialize(char **ptr, size_t *count, size_t *len)
{
	*ptr = NULL;
	*count = 0;
	*len = 1;
	if (BUFFER_SIZE <= 0)
		return (1);
	return (0);
}
