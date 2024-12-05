/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkantzer <lkantzer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:55:35 by lkantzer          #+#    #+#             */
/*   Updated: 2024/12/05 13:57:43 by lkantzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_and_store(int fd, char *reste)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(reste, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		reste = ft_strjoin(reste, buffer);
		if (!reste)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (reste);
}

char	*extract_line(char *reste)
{
	size_t	i;
	char	*line;

	if (!reste || !*reste)
		return (NULL);
	i = 0;
	while (reste[i] && reste[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (reste[i] && reste[i] != '\n')
	{
		line[i] = reste[i];
		i++;
	}
	if (reste[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*update_reste(char *reste)
{
	size_t	i;
	size_t	j;
	char	*new_reste;

	i = 0;
	while (reste[i] && reste[i] != '\n')
		i++;
	if (!reste[i])
	{
		free(reste);
		return (NULL);
	}
	new_reste = malloc(ft_strlen(reste) - i + 1);
	if (!new_reste)
		return (NULL);
	i++;
	j = 0;
	while (reste[i])
		new_reste[j++] = reste[i++];
	new_reste[j] = '\0';
	free(reste);
	return (new_reste);
}

char	*get_next_line(int fd)
{
	static char	*reste;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	reste = read_and_store(fd, reste);
	if (!reste)
		return (NULL);
	line = extract_line(reste);
	reste = update_reste(reste);
	return (line);
}
