/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:25:07 by dmendoza          #+#    #+#             */
/*   Updated: 2025/02/24 17:47:31 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_map	*parse_fdf(char *filename)
{
	FILE	*file;
	char	*line;
	size_t	len;
	ssize_t	read;
	int	i;
	t_map	*map;

	file = fopen(filename. "r");
	if (!file)//open the file error aqui un perror
		exit (1);
	map = (t_map *)malloc(sizeof(t_map));
	if (!map)//malloc error
	{
		fclose(file);
		return (NULL);
	}
	//contar las lineas
	map->height = 0;
	line = get_next_line(fileno(file));
	while (file)
	{
		if (line[0] != '\0' && (line[0] != '\n' || line[1] != '\0'))
			map->height++;
		free(line);
		line = get_next_line(fileno(file));
	}
	//esto quisas no se pude usar
	rewnind(file);

	//memoria para las rows
	map->map = (int **)malloc(sizeof(int *) * map->height);
	if (map->map) //fallo del malloco
	{
		free(map);
		fclose(file);
		return (NULL);
	}
	
	//lee cada linia y popula el mapa
	i = 0;
	line = get_next_line(fileno(file));
	while (line)
	{
		if (line[0] != '\0' && (line[0] != '\n' || line[1] != '\0'))
		{
			map->map[i] = get_line_values(line, &map->width);//propia: encuentra valores
			if (!map->map[i])
			{
				 // Cleanup on error
	    			while (--i >= 0)
					free(map->map[i]);
				free(map->map);
				free(map);
				fclose(file);
				return (NULL);
			}
			i++;
		}
		free(line);
		line = get_next_line(fileno(file));
	}
	//cleanup
	free(line);
	fclose(file);
	return (map);
}
