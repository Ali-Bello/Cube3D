
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "includes/cub3d.h"

void ft_cpy_wall_px(t_img *src, t_img *dst, t_point *src_pos, t_point *dst_pos) {
    char *dst_px;
    char *src_px;

    if (src_pos->x < 0 || src_pos->x >= src->width || src_pos->y < 0 || src_pos->y >= src->height)
        return;
    if (dst_pos->x < 0 || dst_pos->x >= dst->width || dst_pos->y < 0 || dst_pos->y >= dst->height)
        return;
    dst_px = dst->addr + ((int)dst_pos->y * dst->line_len + (int)(dst_pos->x * (dst->bpp / 8)));
    src_px = &src->addr[(int)src_pos->y * src->line_len + (int)(src_pos->x * (src->bpp / 8))];
    memcpy(dst_px, src_px, src->bpp / 8);
}

// Function to initialize a mock image
void init_img(t_img *img, int width, int height, int bpp) {
    img->width = width;
    img->height = height;
    img->line_len = width * (bpp / 8);
    img->bpp = bpp;
    img->addr = (char *)malloc(width * height * (bpp / 8));
    memset(img->addr, 0, width * height * (bpp / 8));
}

// Test function for ft_cpy_wall_px
void test_ft_cpy_wall_px(void *mlx, void *win) {
    t_img src, dst;
    t_point src_pos, dst_pos;
    int line_length = 50;

    // Load the source image from the XPM file
    src.img = mlx_xpm_file_to_image(mlx, "wall.xpm", &src.width, &src.height);
    if (!src.img) {
        printf("Failed to load wall.xpm\n");
        return;
    }
    src.addr = mlx_get_data_addr(src.img, &src.bpp, &src.line_len, &src.endian);

    // Initialize the destination image
    init_img(&dst, 100, 100, 32);

    // Set source and destination positions
    src_pos.x = 10;
    src_pos.y = 10;
    dst_pos.x = 20;
    dst_pos.y = 20;

    // Call the function to test for each pixel in the vertical line
    for (int i = 0; i < line_length; i++) {
        t_point src_pixel = {src_pos.x, src_pos.y + i};
        t_point dst_pixel = {dst_pos.x, dst_pos.y + i};
        ft_cpy_wall_px(&src, &dst, &src_pixel, &dst_pixel);
    }

    // Verify the vertical line was copied correctly
    int passed = 1;
    for (int i = 0; i < line_length; i++) {
        char *src_px = &src.addr[(int)(src_pos.y + i) * src.line_len + (int)src_pos.x * (src.bpp / 8)];
        char *dst_px = &dst.addr[(int)(dst_pos.y + i) * dst.line_len + (int)dst_pos.x * (dst.bpp / 8)];
        if (memcmp(src_px, dst_px, src.bpp / 8) != 0) {
            passed = 0;
            break;
        }
    }

    if (passed) {
        printf("Test passed: Vertical line copied correctly.\n");
    } else {
        printf("Test failed: Vertical line not copied correctly.\n");
    }

    // Create images for display
    void *dst_img = mlx_new_image(mlx, dst.width, dst.height);
    char *dst_data = mlx_get_data_addr(dst_img, &dst.bpp, &dst.line_len, &dst.endian);

    // Copy data to display images
    memcpy(dst_data, dst.addr, dst.width * dst.height * (dst.bpp / 8));

    // Put images to window
    mlx_put_image_to_window(mlx, win, src.img, 0, 0);
    mlx_put_image_to_window(mlx, win, dst_img, 120, 0);

    // Free allocated memory
    free(dst.addr);
    mlx_destroy_image(mlx, src.img);
    mlx_destroy_image(mlx, dst_img);
}

int main() {
    void *mlx;
    void *win;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 240, 100, "Test ft_cpy_wall_px");

    test_ft_cpy_wall_px(mlx, win);

    mlx_loop(mlx);
    return 0;
}