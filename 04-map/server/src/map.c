#include <stdlib.h>
#include "structs.h"

struct map *generate_map(int width, int height) {
    struct map *new_map = (struct map *) calloc(1, sizeof(struct map));
    if (new_map == NULL) 
        return NULL; 

    int *map_blocks = (int *) calloc(width*height, sizeof(int));
    if (map_blocks == NULL) 
        return NULL;

    new_map->width = width;
    new_map->height = height;
    new_map->blocks = map_blocks;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double scale = ((double) rand() / RAND_MAX);
            int block = scale * 3;
            int index = (y*width)+x;
            map_blocks[index] = block;
        }
    }

    fprintf(stdout, "Value of first block -> %d\n", map_blocks[0]);

    return new_map;
}
