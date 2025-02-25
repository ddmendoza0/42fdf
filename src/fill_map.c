/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:32:22 by dmendoza          #+#    #+#             */
/*   Updated: 2025/02/25 15:05:10 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	cleanup_map_error(t_map *map, char *line, FILE *file, int i)
{
	while (--i >= 0)
		free(map->map[i]);
	free(map->map);
	free(map);
	free(line);
	fclose(file);
}

static int	*extract_values(char *line, int count)
{
	int		*values;
	char	*token;
	int		i;
	char	*tmp_line;

	values = (int *)malloc(sizeof(int) * count);
	if (!values)
		return (NULL);
	tmp_line = ft_strdup(line);
	token = ft_strtok(tmp_line, " \t\n");
	i = 0;
	while (token)
	{
		values[i++] = ft_atoi(token);
		token = ft_strtok(NULL, " \t\n");
	}
	free(tmp_line);
	return (values);
}

static int	count_values(char *line)
{
	char	*token;
	int		count;
	char	*tmp_line;

	count = 0;
	tmp_line = ft_strdup(line);
	token = ft_strtok(NULL, " \t\n");
	while (token)
	{
		count++;
		token = ft_strtok(NULL, " \t\n");
	}
	free(tmp_line);
	return (count);
}

static int	*get_line_values(char *line, int *width)
{
	int	count;
	int	*values;

	count = count_values(line);
	values = extract_values(line, count);
	*width = count;
	return (values);
}

t_map	*fill_map_data(t_map *map, FILE *file)
{
	char	*line;
	int		i;

	i = 0;
	line = get_next_line(fileno(file));
	while (line)
	{
		if (line[0] != '\0' && (line[0] != '\n' || line[1] != '\0'))
		{
			map->map[i] = get_line_values(line, &map->width);
			if (!map->map[i])
			{
				cleanup_map_error(map, line, file, i);
				return (NULL);
			}
			i++;
		}
		free(line);
		line = get_next_line(fileno(file));
	}
	free(line);
	fclose(file);
	return (map);
}
