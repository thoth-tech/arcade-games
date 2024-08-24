#include "game.h"
#include "splashkit.h"

void process_debugging(const game_data &game, vector<string> &old_debug_message, bool &still_waiting)
{
    // Get current debugging info
    vector<string> new_debug_message = get_verbose_debugging_message(game);

    // Only send the debug info if the game state has changed since last cycle
    if (old_debug_message != new_debug_message)
    {
        for (int i = 0; i < new_debug_message.size(); i++)
        {
            write_line(new_debug_message[i]);
        }
        still_waiting = false;
    }
    else if (!still_waiting)
    {
        write_line("Waiting for game state to change...");
        still_waiting = true;
    }

    // Save the current cycle's message for comparison in the next cycle
    old_debug_message = new_debug_message;
}

vector<string> get_verbose_debugging_message(const game_data &game)
{
    vector<string> message = {};

    message.push_back("===== Tile Info =====");
    message.push_back("Tile x loc: "+ std::to_string(static_cast<int>(game.solid[29].x)));
    message.push_back("Tile y loc: "+ std::to_string(static_cast<int>(game.solid[29].y)));
    message.push_back("tile x id: "   + std::to_string(static_cast<int>(game.solid[29].x/TILESIZE)));
    message.push_back("tile y id: "   + std::to_string(static_cast<int>(game.solid[29].y/TILESIZE)));
    message.push_back("=====================");
    message.push_back("");
    message.push_back("==== Player Info =====");
    message.push_back("Player y_pos: "+ std::to_string(sprite_y(game.player.player_sprite)));
    message.push_back("Spider y pos: "+ std::to_string(sprite_y(game.enemies[0].enemy_sprite)));
    message.push_back("Player x id: "+ std::to_string(game.player.x_id));
    message.push_back("Player y id: "+ std::to_string(game.player.y_id));
    message.push_back("Player x pos: "+ std::to_string(game.player.x_pos));
    message.push_back("Player y pos: "+ std::to_string(game.player.y_pos));
    message.push_back("Player x prev: "+ std::to_string(game.player.x_prev));
    message.push_back("Player y prev: "+ std::to_string(game.player.y_prev));
    message.push_back("Player next: "+ std::to_string(game.player.next));
    message.push_back("Player up next: "+ std::to_string(game.player.up_next));
    message.push_back("Player down next: "+ std::to_string(game.player.down_next));
    message.push_back("Player left next: "+ std::to_string(game.player.left_next));
    message.push_back("Player right next: "+ std::to_string(game.player.right_next));
    message.push_back("Gems: "+ std::to_string(game.gems.size()));
    message.push_back("=======================");
    message.push_back("");
    message.push_back("====== Box Info 1 ======");
    message.push_back("Box x pos: "+ std::to_string(game.boxes[0].x_pos));
    message.push_back("Box y pos: "+ std::to_string(game.boxes[0].y_pos));
    message.push_back("Box x prev: "+ std::to_string(game.boxes[0].x_prev));
    message.push_back("Box y prev: "+ std::to_string(game.boxes[0].y_prev));
    message.push_back("Box next: "+ std::to_string(game.boxes[0].next));
    message.push_back("Box x id: "+ std::to_string(game.boxes[0].x_id));
    message.push_back("Box y id: "+ std::to_string(game.boxes[0].y_id));
    message.push_back("Box up next: "+ std::to_string(game.boxes[0].up_next));
    message.push_back("Box down next: "+ std::to_string(game.boxes[0].down_next));
    message.push_back("Box left next: "+ std::to_string(game.boxes[0].left_next));
    message.push_back("Box right next: "+ std::to_string(game.boxes[0].right_next));
    message.push_back("Box up stopped: "+ std::to_string(game.boxes[0].up_stopped));
    message.push_back("Box down stopped: "+ std::to_string(game.boxes[0].down_stopped));
    message.push_back("Box left stopped: "+ std::to_string(game.boxes[0].left_stopped));
    message.push_back("Box right stopped: "+ std::to_string(game.boxes[0].right_stopped));
    message.push_back("=======================");
    message.push_back("");
    message.push_back("====== Box Info 2 ======");
    message.push_back("Box x pos: "+ std::to_string(game.boxes[1].x_pos));
    message.push_back("Box y pos: "+ std::to_string(game.boxes[1].y_pos));
    message.push_back("Box x id: "+ std::to_string(game.boxes[1].x_id));
    message.push_back("Box y id: "+ std::to_string(game.boxes[1].y_id));
    message.push_back("Box up next: "+ std::to_string(game.boxes[1].up_next));
    message.push_back("Box down next: "+ std::to_string(game.boxes[1].down_next));
    message.push_back("Box left next: "+ std::to_string(game.boxes[1].left_next));
    message.push_back("Box right next: "+ std::to_string(game.boxes[1].right_next));
    message.push_back("=======================");
    message.push_back("");
    message.push_back("====== Gems ======");
    message.push_back("Gem x id: "+ std::to_string(game.gems[1].x_id));
    message.push_back("Gem y id: "+ std::to_string(game.gems[1].y_id));
    message.push_back("=======================");

    return message;
}