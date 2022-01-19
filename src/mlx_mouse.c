/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mlx_mouse.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: W2Wizard <w2.wizzard@gmail.com>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/01 23:20:13 by W2Wizard      #+#    #+#                 */
/*   Updated: 2022/01/18 20:10:34 by W2Wizard      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42_Int.h"

bool	mlx_is_mouse_down(t_mlx *mlx, t_mouse_key key)
{
	return (glfwGetMouseButton(mlx->window, key) == GLFW_PRESS);
}

void	mlx_set_mouse_pos(t_mlx *mlx, int32_t x, int32_t y)
{
	glfwSetCursorPos(mlx->window, (double)x, (double)y);
}

void	mlx_get_mouse_pos(t_mlx *mlx, int32_t *x_out, int32_t *y_out)
{
	double	pos[2];

	glfwGetCursorPos(mlx->window, &pos[0], &pos[1]);
	*x_out = (int32_t)pos[0];
	*y_out = (int32_t)pos[1];
}