/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:36:03 by dmendoza          #+#    #+#             */
/*   Updated: 2025/05/17 14:40:34 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	draw_pixel(mlx_image_t *img, int x, int y, uint32_t color)
{
	if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
		mlx_put_pixel(img, x, y, color);
}

void	draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx;
	int dy;
	int sx;
	int sy;
	int err;
	int e2;

	dx = abs(x1 - x0);
	dy = -abs(y1 - y0);
	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;
	err = dx + dy;

	while (1)
	{
		draw_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			if (x0 == x1)
				break;
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			if (y0 == y1)
				break;
			err += dx;
			y0 += sy;
		}
	}
}

int	calculate_optimal_scale(t_map *map, int width, int height)
{
	int i;
	int j;
	int x;
	int y;
	int max_width;
	int max_height;
	double angle;
	int scale_x;
	int scale_y;
	int scale;

	max_width = 0;
	max_height = 0;
	angle = 0.8;
	i = -1;
	while (++i < map->height)
//    for (i = 0; i < map->height; i++)
	{
		j = -1;
		while (++j < map->width)
//        for (j = 0; j < map->width; j++)
		{
			x = (j - i) * cos(angle);
			y = (j + i) * sin(angle);
			if (abs(x) > max_width) max_width = abs(x);
			if (abs(y) > max_height) max_height = abs(y);
		}
	}
// Calculate appropriate scale to fit in window (leaving 200px margin)
	scale_x = (width - 200) / (max_width * 2);
	scale_y = (height - 200) / (max_height * 2);
	if (scale_x < scale_y)
		scale = scale_x; 
	else
		scale = scale_y;
// Set reasonable bounds for scale
	if (scale < 1)
		scale = 1;  // Minimum scale
	if (scale > 50)
		scale = 50; // Maximum scale
    return scale;
}

void	render_map(t_fdf *fdf)
{
	int i;
	int j;
	unsigned int x;
	unsigned int y;
	int x0;
	int y0;
	int x1;
	int y1;
	double angle;

	angle = 0.8;
// clear image image
	y = 0;
	while (y < fdf->img->height)
	{
		x = 0;
		while (x < fdf->img->width)
            		mlx_put_pixel(fdf->img, x++, y, 0x00000000);
		y++;
	}
// Render the map
	//for (i = 0; i < fdf->map->height; i++)
	i = -1;
	while (++i < fdf->map->height)
	{
		j = -1;
		while (++j < fdf->map->width)
		//for (j = 0; j < fdf->map->width; j++)
		{
			// Isometric projection
			x0 = (j - i) * cos(angle) * fdf->scale + fdf->width / 2 + fdf->offset_x;
			y0 = (j + i) * sin(angle) * fdf->scale - fdf->map->map[i][j] \
			     * fdf->scale / 4 + fdf->height / 2 + fdf->offset_y;
			// Draw line to right neighbor
			if (j < fdf->map->width - 1)
			{
				x1 = ((j + 1) - i) * cos(angle) * fdf->scale + \
				     fdf->width / 2 + fdf->offset_x;
				y1 = ((j + 1) + i) * sin(angle) * fdf->scale - fdf->map->map[i]\
				     [j + 1] * fdf->scale / 4 + fdf->height / 2 + fdf->offset_y;
				draw_line(fdf->img, x0, y0, x1, y1, 0xFFFFFFFF);
			}
			// Draw line to bottom neighbor
			if (i < fdf->map->height - 1)
			{
				x1 = (j - (i + 1)) * cos(angle) * fdf->scale + fdf->width\
				     / 2 + fdf->offset_x;
				y1 = (j + (i + 1)) * sin(angle) * fdf->scale - fdf->map->map[i + 1]\
				     [j] * fdf->scale / 4 + fdf->height / 2 + fdf->offset_y;
				draw_line(fdf->img, x0, y0, x1, y1, 0xFFFFFFFF);
			}
		}
	}
}

void	key_hook(mlx_key_data_t k, void *param)
{
	t_fdf *fdf = param;
	
//exit - think mem leaks from here
	if (k.key == MLX_KEY_ESCAPE && k.action == MLX_PRESS)
		mlx_close_window(fdf->mlx);
// Zoom controls - using = key for zoom in (plus sign on same key)
	if ((k.key == MLX_KEY_EQUAL || k.key == MLX_KEY_KP_ADD) && k.action == MLX_PRESS)
		fdf->scale += 2;
	if ((k.key == MLX_KEY_MINUS || k.key == MLX_KEY_KP_SUBTRACT) && k.action == \
		       	MLX_PRESS && fdf->scale > 1)
		fdf->scale -= 2;
// Panning controls
	if (k.key == MLX_KEY_DOWN && (k.action == MLX_REPEAT || k.action == MLX_PRESS))
		fdf->offset_y -= 20;
	if (k.key == MLX_KEY_UP && (k.action == MLX_REPEAT || k.action == MLX_PRESS))
		fdf->offset_y += 20;
	if (k.key == MLX_KEY_RIGHT && (k.action == MLX_REPEAT || k.action == MLX_PRESS))
		fdf->offset_x -= 20;
	if (k.key == MLX_KEY_LEFT && (k.action == MLX_REPEAT || k.action == MLX_PRESS))
		fdf->offset_x += 20;
// Reset view
	if (k.key == MLX_KEY_R && k.action == MLX_PRESS)
	{
		fdf->scale = calculate_optimal_scale(fdf->map, fdf->width, fdf->height);
		fdf->offset_x = 0;
		fdf->offset_y = -((fdf->map->width + fdf->map->height) * sin(0.8) * fdf->scale) / 2;
	}
// Redraw with new parameters
	render_map(fdf);
}

int	start_rendering(t_map *map)
{
	t_fdf fdf;

	fdf.width = 1800;
	fdf.height = 1800;
	fdf.map = map;
	fdf.scale = calculate_optimal_scale(map, fdf.width, fdf.height);
// Calculate optimal offsets to center the map
	fdf.offset_x = 0;
	fdf.offset_y = -((fdf.map->width + fdf.map->height) * sin(0.8) * fdf.scale) / 2;
//screen initiation
	fdf.mlx = mlx_init(fdf.width, fdf.height, "FDF - Wireframe Model", false);
	if (!fdf.mlx)
		return (1);
	fdf.img = mlx_new_image(fdf.mlx, fdf.width, fdf.height);
	if (!fdf.img)
	{
		mlx_terminate(fdf.mlx);
		return (1);
	}
	if (mlx_image_to_window(fdf.mlx, fdf.img, 0, 0) == -1)
	{
		mlx_delete_image(fdf.mlx, fdf.img);
		mlx_terminate(fdf.mlx);
		return (1);
	}
//draw the map
	render_map(&fdf);
//reed inputs
	mlx_key_hook(fdf.mlx, key_hook, &fdf);
	mlx_loop(fdf.mlx);
//exiting the program
	mlx_delete_image(fdf.mlx, fdf.img);
	mlx_terminate(fdf.mlx);
	return (0);
}
