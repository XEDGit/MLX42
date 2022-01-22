/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mlx_images.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: W2Wizard <w2.wizzard@gmail.com>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/21 15:34:45 by W2Wizard      #+#    #+#                 */
/*   Updated: 2022/01/22 16:21:30 by W2Wizard      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42_Int.h"

// Reference: https://bit.ly/3KuHOu1 (Matrix View Projection)
static void	mlx_draw_texture(t_mlx_image *img, t_mlx_ctx *mlxctx, \
t_mlx_image_ctx *imgctx, t_mlx *mlx)
{
	const int32_t	w = img->width;
	const int32_t	h = img->height;
	const float		matrix[16] = {
		2.f / mlx->width, 0, 0, 0,
		0, 2. / -mlx->height, 0, 0,
		0, 0, -2. / (1000. - -1000.), 0,
		-1, -(mlx->height / -mlx->height),
		-((1000. + -1000.) / (1000. - -1000.)), 1
	};

	glUseProgram(mlxctx->shaderprogram);
	glUniformMatrix4fv(glGetUniformLocation(mlxctx->shaderprogram, \
	"proj_matrix"), 1, GL_FALSE, matrix);
	glUniform1i(glGetUniformLocation(mlxctx->shaderprogram, "outTexture"), 0);
	glBindVertexArray(mlxctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mlxctx->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vert) * 6, &imgctx->vertices, \
	GL_STATIC_DRAW);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, \
	GL_UNSIGNED_BYTE, img->pixels);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void	mlx_draw_image(t_mlx *mlx, t_mlx_image *img, int32_t x, int32_t y)
{
	t_mlx_ctx		*mlxctx;
	t_mlx_image_ctx	*imgctx;
	const int32_t	w = img->width;
	const int32_t	h = img->height;
	const int32_t	z = img->depth;

	img->x = x;
	img->y = y;
	mlxctx = mlx->context;
	imgctx = img->context;
	imgctx->vertices[0] = (t_vert){x, y, z, 0.f, 0.f};
	imgctx->vertices[1] = (t_vert){x + w, y + h, z, 1.f, 1.f};
	imgctx->vertices[2] = (t_vert){x + w, y, z, 1.f, 0.f};
	imgctx->vertices[3] = (t_vert){x, y, z, 0.f, 0.f};
	imgctx->vertices[4] = (t_vert){x, y + h, z, 0.f, 1.f};
	imgctx->vertices[5] = (t_vert){x + w, y + h, z, 1.f, 1.f};
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, imgctx->texture);
	mlx_draw_texture(img, mlxctx, imgctx, mlx);
}

t_mlx_image	*mlx_new_image(t_mlx *mlx, uint16_t width, uint16_t height)
{
	t_mlx_image		*newimg;
	t_mlx_image_ctx	*newctx;

	newimg = calloc(1, sizeof(t_mlx_image));
	newctx = calloc(1, sizeof(t_mlx_image_ctx));
	if (!newimg || !newctx)
		return (NULL);
	newimg->width = width;
	newimg->height = height;
	newimg->context = newctx;
	newimg->pixels = malloc(width * height * sizeof(int32_t));
	if (!newimg->pixels)
		return ((void *)mlx_free_va(false, 2, newimg, newctx));
	glGenTextures(1, &newctx->texture);
	glBindTexture(GL_TEXTURE_2D, newctx->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, \
	GL_UNSIGNED_BYTE, newimg->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	mlx_lstadd_back((t_mlx_list **)(&mlx->images), mlx_lstnew(newimg));
	return (newimg);
}