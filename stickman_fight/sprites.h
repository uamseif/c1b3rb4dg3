#ifndef SPRITES_H
#define SPRITES_H

#include "sprites/P1/standing.c"
#include "sprites/P1/standing2.c"
#include "sprites/P1/walking.c"
#include "sprites/P1/walking2.c"
#include "sprites/P1/punching.c"
#include "sprites/P1/punching2.c"
#include "sprites/P1/jumping.c"

#include "sprites/P2/standing.c"
#include "sprites/P2/standing2.c"
#include "sprites/P2/walking.c"
#include "sprites/P2/walking2.c"
#include "sprites/P2/punching.c"
#include "sprites/P2/punching2.c"
#include "sprites/P2/jumping.c"

static const unsigned char * standingSpritesP1[] = {standingP1, standing2P1};
static const unsigned char * walkingSpritesP1[] = {standingP1, walkingP1, walking2P1, walkingP1};
static const unsigned char * punchingSpritesP1[] = {punchingP1, punching2P1};

static const unsigned char * standingSpritesP2[] = {standingP2, standing2P2};
static const unsigned char * walkingSpritesP2[] = {standingP2, walkingP2, walking2P2, walkingP2};
static const unsigned char * punchingSpritesP2[] = {punchingP2, punching2P2};

#endif