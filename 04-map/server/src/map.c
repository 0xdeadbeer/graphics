#include <stdlib.h>
#include "defs.h"
#include "structs.h"

const int grass_probability = 6; 
const int cobble_probability = 8; 
const int lava_probability = 10;

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
            int index = (y*width)+x;

            if (x == 0) {
                map_blocks[index] = COBBLE_BLOCK; 
                continue;
            }

            if (y == 0) {
                map_blocks[index] = COBBLE_BLOCK;
                continue;
            }

            if (x == width-1) {
                map_blocks[index] = COBBLE_BLOCK;
                continue;
            }

            if (y == height-1) {
                map_blocks[index] = COBBLE_BLOCK; 
                continue;
            }

            int scale = rand() % 10;

            if (scale <= grass_probability) {
                map_blocks[index] = GRASS_BLOCK;
                continue;
            }

            if (scale <= cobble_probability) {
                map_blocks[index] = COBBLE_BLOCK; 
                continue;
            }

            if (scale <= lava_probability) {
                map_blocks[index] = LAVA_BLOCK;
                continue;
            }

            map_blocks[index] = GRASS_BLOCK; // paranoid developer
        }
    }

    fprintf(stdout, "Value of first block -> %d\n", map_blocks[0]);

    return new_map;
}
