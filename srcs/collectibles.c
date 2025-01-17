/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collectibles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:55:28 by aderraj           #+#    #+#             */
/*   Updated: 2025/01/17 16:56:05 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int compare_collectibles(const void *a, const void *b)
{
    t_sprite *sprite_a = (t_sprite *)a;
    t_sprite *sprite_b = (t_sprite *)b;

    if (sprite_a->casted.distance < sprite_b->casted.distance)
        return 1;
    else if (sprite_a->casted.distance > sprite_b->casted.distance)
        return -1;
    else
        return 0;
}

void    set_sprites_distances(t_game *game)
{
    int i;

    i = -1;
    while (++i < game->collectibles_count)
    {
        game->collectibles[i].casted.distance = distance_from_origin(&game->player, game->collectibles[i].x, game->collectibles[i].y) *
                        cosf(atan2f(game->collectibles[i].y - game->player.y, game->collectibles[i].x - game->player.x) - game->player.angle.rad);
    }
    qsort(game->collectibles, game->collectibles_count, sizeof(t_sprite), compare_collectibles);
}

void    play_coin_sond(void)
{
    pid_t pid = fork();  // Create a new process

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }
    else if (pid == 0)
    {
        system("cvlc --play-and-exit --quiet coin.mp3 > /dev/null 2>&1");
        exit(EXIT_FAILURE);
    }
}

void    update_collectibles(t_game *game)
{
    int i;

    i = 0;
    set_sprites_distances(game);
    while (i < game->collectibles_count)
    {
        if (game->collectibles[i].is_visible && game->collectibles[i].casted.distance <= 10.0)
        {
            game->collectibles_collected++;
            game->collectibles[i].is_visible = false;
            game->collectibles[i].x = -1;
            game->collectibles[i].y = -1;
            play_coin_sond();
        }
        else if (game->collectibles[i].is_visible)
        {
            if (game->collectibles[i].offset_x >= game->collectibles[i].img.width)
                game->collectibles[i].offset_x = 0;
            set_sprite_dimensions(game, &game->collectibles[i]);
            draw_sprite(game, &game->collectibles[i]);
            if (game->collectibles[i].delta % 20  == 0)
                game->collectibles[i].offset_x += game->collectibles[i].frame_width;
            game->collectibles[i].delta++;
        }
        i++;
    }
}