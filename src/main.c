/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:01:19 by dmendoza          #+#    #+#             */
/*   Updated: 2025/02/20 16:05:30 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bytes Per Pixel. Since each pixel is represented as an integer, it will be four bytes for four channels.
#define BPP sizeof(int32_t)

static void	ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

/* --------------------------------------------------*/
/* --                     HOOKS                    --*/
//generic hook
//usually faster than dedicated hooks
//refresh every x time checking for states 
/*static void	ft_hook(void *param)
{
	const mlx_t	*mlx = param;
	printf("WIDTH: %d | HEIGHT: %d\n", mlx->width, mlx->height);
}*/

//hook for scroll input
void my_scrollhook(double xdelta, double ydelta, void *param)
{
	mlx_image_t *img = (mlx_image_t *)param;
	// Simple up or down detection. NOW WITH MOVEMENT!!
	if (ydelta > 0)
	{
		img->instances[0].x += 5;
		img->instances[0].y += 5;
		puts("Up!");
	}
	else if (ydelta < 0)
	{
		img->instances[0].x -= 5;
		img->instances[0].y -= 5;
		puts("Down!");
	}
	//esto no funciona, tapoco se como trigger (creo que no se pue con 42 ratones
	// Can also detect a mousewheel that goes along the X (e.g: MX Master 3)
	if (xdelta < 0)
		puts("Sliiiide to the left!");
	else if (xdelta > 0)
		puts("Sliiiide to the right!");
}

//hook for key inputs
//slower than generic but more control
void my_keyhook(mlx_key_data_t keydata, void* param)
{
	(void)param;
	// If we PRESS the 'J' key, print "Hello".
	if (keydata.key == MLX_KEY_J && keydata.action == MLX_PRESS)
		puts("Hello ");

	// If we RELEASE the 'K' key, print "World".
	if (keydata.key == MLX_KEY_K && keydata.action == MLX_RELEASE)
		puts("World");

	// If we HOLD the 'L' key, print "!".
	if (keydata.key == MLX_KEY_L && keydata.action == MLX_REPEAT)
		puts("!");
}
/* --              END HOOKS           -- */
/* -------------------------------------- */

/* -------------------------------------- */
/* --               MAIN               -- */
int32_t	main(void)
{
	/* -- MLX INIT -- */
	mlx_set_setting(MLX_MAXIMIZED, true);
	//start mlx
	mlx_t *mlx = mlx_init(256, 256, "fdf", true); //inicia un canvas
	if (!mlx)
		ft_error();

	/* -- IMAGE CREATION --*/
	//create a new image
	mlx_image_t	*img = mlx_new_image(mlx, 128, 128); //crea una imagen nueva
	if (!img)
		ft_error();

	/* -- IMAGE DYSPLAY -- */
	// Set every pixel to white
	memset(img->pixels, 255, img->width * img->height * sizeof(int32_t)); //esto crea un cuadrado?
	// Display an instance of the image
	if (mlx_image_to_window(mlx, img, 0, 0)) // show the image
		ft_error();
	// Modify the x & y position of an already existing instance.
	img->instances[0].x += 5;
	img->instances[0].y += 5;
	//mlx_delete_image(mlx, img); // deletes specified image

	/* -- HOOKS -- */
	// Register a hook and pass mlx as an optional param.
	//mlx_loop_hook(mlx, ft_hook, mlx); // generic hook call
	mlx_scroll_hook(mlx, &my_scrollhook, img); // scrollhook call
	mlx_key_hook(mlx, &my_keyhook, NULL); // keyhook call
	
	/* -- START THE LOOP -- */
	mlx_loop(mlx); // call loop start
	
	/* -- END THE LOOP -- */
	mlx_terminate(mlx); // call loop end
	return (EXIT_SUCCESS);
}
