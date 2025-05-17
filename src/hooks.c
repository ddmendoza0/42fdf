/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:19:57 by dmendoza          #+#    #+#             */
/*   Updated: 2025/05/17 15:25:20 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

