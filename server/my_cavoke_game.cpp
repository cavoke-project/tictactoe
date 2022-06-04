#include "cavoke.h"

namespace cavoke {

bool validate_settings(
    const json &settings, const std::vector<int> &occupied_positions,
    const std::function<void(std::string)> &message_callback) {
  // TODO: Implement your game validation here
  return true;
}

GameState init_state(const json &settings,
                     const std::vector<int> &occupied_positions) {
  // TODO: Implement your game start here

  return GameState{false, "<INIT_STATE>", {}, {}};
}

GameState apply_move(GameMove &new_move) {
  // TODO: Implement your game move event processing here

  return GameState{false, "<GLOBAL_STATE>", {}, {}};
}
} // namespace cavoke
