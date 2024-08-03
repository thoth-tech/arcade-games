#ifndef _DEBUGGING
#define _DEBUGGING

#include "splashkit.h"

void process_debugging(const game_data &game, vector<string> &old_debug_message, bool &still_waiting);

vector<string> get_verbose_debugging_message(const game_data &game);

#endif 