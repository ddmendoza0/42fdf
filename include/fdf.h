/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:01:33 by dmendoza          #+#    #+#             */
/*   Updated: 2025/05/17 14:34:57 by dmendoza         ###   ########.fr       */
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

typedef struct {
	mlx_t *mlx;
	mlx_image_t *img;
	t_map *map;
	int scale;
	int offset_x;
	int offset_y;
	int width;
	int height;
} t_fdf;

t_map	*parse_fdf(char *filename);
t_map	*fill_map_data(t_map *map, FILE *file);
int start_rendering(t_map *map);

#endif
