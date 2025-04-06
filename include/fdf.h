/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:01:33 by dmendoza          #+#    #+#             */
/*   Updated: 2025/02/26 17:23:13 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft.h"
# include "MLX42/MLX42.h"
# include <stdio.h>
# include <math.h>

typedef struct s_map
{
	int	width;
	int	height;
	int	**map;
}	t_map;

t_map	*parse_fdf(char *filename);
t_map	*fill_map_data(t_map *map, FILE *file);
int start_rendering(t_map *map);

#endif
