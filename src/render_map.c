/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:36:03 by dmendoza          #+#    #+#             */
/*   Updated: 2025/02/26 17:23:58 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define DEFAULT_SCALE 20

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

void	draw_pixel(mlx_image_t *img, int x, int y, uint32_t color)
{
	if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
		mlx_put_pixel(img, x, y, color);
}

void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

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

int calculate_optimal_scale(t_map *map, int width, int height)
{
    int i, j;
    int max_width = 0;
    int max_height = 0;
    double angle = 0.8;
    
    for (i = 0; i < map->height; i++)
    {
        for (j = 0; j < map->width; j++)
        {
            int x = (j - i) * cos(angle);
            int y = (j + i) * sin(angle);
            if (abs(x) > max_width) max_width = abs(x);
            if (abs(y) > max_height) max_height = abs(y);
        }
    }
    
    // Calculate appropriate scale to fit in window (leaving 200px margin)
    int scale_x = (width - 200) / (max_width * 2);
    int scale_y = (height - 200) / (max_height * 2);
    int scale = scale_x < scale_y ? scale_x : scale_y;
    
    // Set reasonable bounds for scale
    if (scale < 1) scale = 1;  // Minimum scale
    if (scale > 50) scale = 50; // Maximum scale
    
    return scale;
}

void render_map(t_fdf *fdf)
{
    int i, j;
    int x0, y0, x1, y1;
    double angle = 0.8;
    
    // Clear the image
    for (unsigned int y = 0; y < fdf->img->height; y++)
    {
        for (unsigned int x = 0; x < fdf->img->width; x++)
        {
            mlx_put_pixel(fdf->img, x, y, 0x00000000);
        }
    }
    
    // Render the map
    for (i = 0; i < fdf->map->height; i++)
    {
        for (j = 0; j < fdf->map->width; j++)
        {
            // Isometric projection
            x0 = (j - i) * cos(angle) * fdf->scale + fdf->width / 2 + fdf->offset_x;
            y0 = (j + i) * sin(angle) * fdf->scale - fdf->map->map[i][j] * fdf->scale / 4 + fdf->height / 2 + fdf->offset_y;
            
            // Draw line to right neighbor
            if (j < fdf->map->width - 1)
            {
                x1 = ((j + 1) - i) * cos(angle) * fdf->scale + fdf->width / 2 + fdf->offset_x;
                y1 = ((j + 1) + i) * sin(angle) * fdf->scale - fdf->map->map[i][j + 1] * fdf->scale / 4 + fdf->height / 2 + fdf->offset_y;
                draw_line(fdf->img, x0, y0, x1, y1, 0xFFFFFFFF);
            }
            
            // Draw line to bottom neighbor
            if (i < fdf->map->height - 1)
            {
                x1 = (j - (i + 1)) * cos(angle) * fdf->scale + fdf->width / 2 + fdf->offset_x;
                y1 = (j + (i + 1)) * sin(angle) * fdf->scale - fdf->map->map[i + 1][j] * fdf->scale / 4 + fdf->height / 2 + fdf->offset_y;
                draw_line(fdf->img, x0, y0, x1, y1, 0xFFFFFFFF);
            }
        }
    }
}

void key_hook(mlx_key_data_t keydata, void *param)
{
    t_fdf *fdf = param;
    
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        mlx_close_window(fdf->mlx);

    // Zoom controls - using = key for zoom in (plus sign on same key)
    if ((keydata.key == MLX_KEY_EQUAL || keydata.key == MLX_KEY_KP_ADD) && keydata.action == MLX_PRESS)
        fdf->scale += 2;
    if ((keydata.key == MLX_KEY_MINUS || keydata.key == MLX_KEY_KP_SUBTRACT) && keydata.action == MLX_PRESS && fdf->scale > 1)
        fdf->scale -= 2;
        
    // Panning controls
    if (keydata.key == MLX_KEY_DOWN && (keydata.action == MLX_REPEAT || keydata.action == MLX_PRESS))
        fdf->offset_y -= 20;
    if (keydata.key == MLX_KEY_UP && (keydata.action == MLX_REPEAT || keydata.action == MLX_PRESS))
        fdf->offset_y += 20;
    if (keydata.key == MLX_KEY_RIGHT && (keydata.action == MLX_REPEAT || keydata.action == MLX_PRESS))
        fdf->offset_x -= 20;
    if (keydata.key == MLX_KEY_LEFT && (keydata.action == MLX_REPEAT || keydata.action == MLX_PRESS))
        fdf->offset_x += 20;
    
    // Reset view
    if (keydata.key == MLX_KEY_R && keydata.action == MLX_PRESS)
    {
        fdf->scale = calculate_optimal_scale(fdf->map, fdf->width, fdf->height);
	fdf->offset_x = 0;
	fdf->offset_y = -((fdf->map->width + fdf->map->height) * sin(0.8) * fdf->scale) / 2;
    }
    
    // Toggle fullscreen
    /*if (keydata.key == MLX_KEY_F && keydata.action == MLX_PRESS)
    {
        mlx_set_setting(MLX_FULLSCREEN, !mlx_get_window_attributes(fdf->mlx)->fullscreen);
    }
    */
    // Redraw with new parameters
    render_map(fdf);
}

// This function gets the primary monitor's resolution
void get_monitor_resolution(int *width, int *height)
{
    // Get monitor count
    mlx_get_monitor_size(0, width, height);
    // If we couldn't get a valid resolution, use fallback values
    if (*width <= 0 || *height <= 0)
    {
        *width = 1800;
        *height = 1800;
    }
}

int start_rendering(t_map *map)
{
    t_fdf fdf;
    int width, height;
    
    // Get the monitor resolution
    get_monitor_resolution(&width, &height);
    
    // Store window dimensions in fdf struct
    fdf.width = width;
    fdf.height = height;
   
    // Calculate optimal scale for the map
    int optimal_scale = calculate_optimal_scale(map, width, height);
    
    fdf.map = map;
    fdf.scale = optimal_scale;
    // Calculate optimal offsets to center the map
	fdf.offset_x = 0;
	fdf.offset_y = -((fdf.map->width + fdf.map->height) * sin(0.8) * fdf.scale) / 2;
    
    // Initialize window to fullscreen
    mlx_set_setting(MLX_MAXIMIZED, false);
    fdf.mlx = mlx_init(width, height, "FDF - Wireframe Model", false);
    if (!fdf.mlx)
        return (1);
    
    fdf.img = mlx_new_image(fdf.mlx, width, height);
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
    
    render_map(&fdf);
    mlx_key_hook(fdf.mlx, key_hook, &fdf);
    mlx_loop(fdf.mlx);
    
    mlx_delete_image(fdf.mlx, fdf.img);
    mlx_terminate(fdf.mlx);
    return (0);
}
