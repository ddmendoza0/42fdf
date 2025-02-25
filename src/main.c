/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:58:03 by dmendoza          #+#    #+#             */
/*   Updated: 2025/02/25 15:06:08 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	free_map(t_map *map)
{
	int	i;

	if (!map)
		return ;
	if (map->map)
	{
		i = -1;
		while (++i < map->height)
		{
			if (map->map[i])
				free(map->map[i]);
		}
		free(map->map);
	}
	free(map);
}

void	print_map(t_map *map)
{
	int	i;
	int	j;

	if (!map)
		return ;
	printf("Map dimensions: %d x %d\n", map->width, map->height);
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
			printf("%3d", map->map[i][j]);
		printf("\n");
	}
}

int	main(int argc, char **argv)
{
	t_map	*map;

	if (argc != 2)
		return (1);
	map = parse_fdf(argv[1]);
	if (!map)
		return (1);
	print_map(map);
	free_map(map);
	return (0);
}
