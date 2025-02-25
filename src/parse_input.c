/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inut.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:06:49 by dmendoza          #+#    #+#             */
/*   Updated: 2025/02/25 15:05:35 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_map_height(FILE *file)
{
	char	*line;
	int		height;

	height = 0;
	line = get_next_line(fileno(file));
	while (line)
	{
		if (line[0] != '\0' && (line[0] != '\n' || line[1] != '\0'))
			height++;
		free(line);
		line = get_next_line(fileno(file));
	}
	rewind(file);
	return (height);
}

static t_map	*init_map(FILE *file)
{
	t_map	*map;

	map = (t_map *)malloc(sizeof(t_map));
	if (!map)
	{
		fclose(file);
		return (NULL);
	}
	map->height = count_map_height(file);
	map->map = (int **)malloc(sizeof(int *) * map->height);
	if (!map->map)
	{
		free(map);
		fclose(file);
		return (NULL);
	}
	return (map);
}

t_map	*parse_fdf(char *filename)
{
	FILE	*file;
	t_map	*map;

	file = fopen(filename, "r");
	if (!file)
	{
		perror("Error opening file.");
		return (NULL);
	}
	map = init_map(file);
	if (!map)
		return (NULL);
	return (fill_map_data(map, file));
}
