<a href="https://github.com/cavoke-project/cavoke">
  <img align="left" height="80px" src="https://avatars.githubusercontent.com/u/52053547?s=200&v=4">
</a>
<h1 style="display: inline;">
  Cavoke
</h1>

<p>Platform for creating and hosting multiplayer turn-based board games</p>

## Game template

This repository contains a possible template for creating cavoke games. You can read more about the project in
its [main repository](https://github.com/cavoke-project/cavoke).

Every cavoke game consists of three elements:

- **Server logic component** that verifies one's move correctness and updates the game state
- **Client game component**. A [QML](https://doc.qt.io/qt-6/qmlapplications.html) application that does the UI
- **Configuration file** with game info (display name, number of players, etc)

### Checklist

- [ ] Server logic implemented in [my_cavoke_game.cpp](./server/my_cavoke_game.cpp)
- [ ] Client QML completed and signals connected in [app.qml](./client/app.qml)
- [ ] Configuration file completed in [my_cavoke_game_config.json](./my_cavoke_game_config.json)

### Client

Client game component is a standard QML application that displays your UI and accepts moves from the special **cavoke
object** (see code for details). Here is how we connect a QML component to the main cavoke Qt application:

```qml
...
import "content/interactions.js" as Interact

Rectangle {
    // BEGIN cavoke section
    Connections {
        target: cavoke

        function onReceiveUpdate(jsonUpdate) { // <-- Signal that handles incoming game states
            console.log("Received: " + jsonUpdate);
            Interact.processResponse(jsonUpdate);
        }
    }
    // END cavoke section

    id: game
...
```

And in the `interactions.js`:

```js
function processResponse(response) { // <-- Processes the received state from the server
    let res = JSON.parse(response)
    updateBoard(res["state"]); // Parses the game state UI.
                               // Here res["state"] may look something like "XOO OX X "
}

function sendMove(moveString) {
    let move = {}
    move.move = "X" + moveString
    cavoke.getMoveFromQml(JSON.stringify(move)) // <-- Sends data to the server via Qt
}

function updateBoard(boardString) {
    for (let i = 0; i < 9; ++i) {
        board.children[i].state = boardString[i];
    }
}
```

Developers may decide for themselves how their states will look. They may use json, plain-string, base64. It just has to
be representable as a string. For example, the developers of this game decided to encode the game state as a json
with `state` field that is 9 characters long representing the tictactoe board, i.e. `XOO OX X ` would represent the
following field.

```
XOO
 OX
 X 
```

> :information_source: Notice that QML game component doesn't include any networking, session management, users
> authentication, etc. All of this is handled by the main cavoke client executable. Developers can focus solely on their
> game.
> That's it with the client side! All the other bits and bobs around QML component have to do with UI and not
> administrating the game process.

Here is the template [file](./client/app.qml) for cavoke client component

### Server

A server component is a docker image that must be able to handle 3 requests.

- `VALIDATE` – Check if received game configuration can be used to start it (e.g. minimum players count reached, at
  least 1 player in every team, etc.)
- `INIT` – Create a game session using given configuration. Generates game state for this session. Now players can play.
- `MOVE` – Process user's move (figure move in Chess, new X/O in Tic-Tac-Toe, etc.)

> :warning: Server components have to be *stateless*, because they may be shut down and restarted at any point
> without notice. This means that all game session information must be stored in the game state that is passed onto the
> server component with every `MOVE` request.

In the [server](./server) directory you can see a template for a server component. It is
a [drogon](https://github.com/drogonframework/drogon) server with all
methods and stuff configured. The only thing left for you to do is to implement the 3 methods in
the [my_cavoke_game.cpp](./server/my_cavoke_game.cpp) file

For Tic-Tac-Toe these are written as follows:
So for Tic-Tac-Toe we have something like this:

```cpp
bool validate_settings( // <-- Validates game configuration
    const json &settings,
    const std::vector<int> &occupied_positions,
    const std::function<void(std::string)> &message_callback) {
    if (occupied_positions.size() != 2) {
        message_callback("Not enough players");
        return false;
    }
    if (!settings.contains("board_size")) {
        message_callback("No board_size property");
        return false;
    }
    if (settings["board_size"].get<int>() != 3) {
        message_callback("Only  board_size=3 is supported");
        return false;
    }
    return true;
}
// ...
GameState init_state(const json &settings, // <-- Creates a game session
                     const std::vector<int> &occupied_positions) {
    int board_size = settings["board_size"];
    std::string board(board_size * board_size, ' ');
    return GameState{false, board, {board, board}, {}};
}
// ...
GameState apply_move(GameMove &new_move) { /// <-- Handles move request
    std::string &board = new_move.global_state;
    char player = (new_move.player_id == 0 ? 'X' : 'O');
    if (player != current_player(board)) {
        return {false, board, {board, board}, {}};
    }
    int position = extract_position(new_move.move);
    if (!is_valid_move(board, position)) {
        return {false, board, {board, board}, {}};
    }
    board[position] = player;
    bool win = winner(board);
    std::vector<int> winners;
    if (win) {
        winners.push_back(new_move.player_id);
    }
    return {win, board, {board, board}, winners};
}
```

> :information_source: You can read more about all the game logic
> protocol [here](https://github.com/cavoke-project/cavoke/blob/master/GameLogicProtocol.md)

### Configuration file

Very simple config file. See an example for Tic-Tac-Toe.

```jsonc
{
  "default_settings": { // <-- Default settings for games
    "board_size": 3
  },
  "description": "Paper-and-pencil game for two players who take turns marking the spaces in a three-by-three grid with X or O",
  "display_name": "Tic-tac-toe",
  "id": "tictactoe",
  "players_num": 2,
  "role_names": [
    "Crosses",
    "Noughts"
  ],
  "app_type": "LOCAL", // <-- LOCAL or EXTERNAL (server will look for the logic in its filesystem / send a request to `url`)
  "url": "" // Used only in EXTERNAL mode. Url to a server running the game logic.
}
```

Here is your template file [my_cavoke_game_config.json](./my_cavoke_game_config.json).

---

> [@MarkTheHopeful](https://github.com//MarkTheHopeful) &nbsp;&middot;&nbsp;
> [@waleko](https://github.com/waleko) &nbsp;&middot;&nbsp;
> [@petrtsv](https://github.com/petrtsv) 