#include "state_management.h"
#include "globals.h"

void paddle_update(GameState& g) {
    g.paddle.pos.x = clamp((int) mouse_x(), GAME_AREA_START, GAME_AREA_END - g.paddle.width);
}
