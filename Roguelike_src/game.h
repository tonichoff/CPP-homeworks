#include "actor.h"
#include <vector>
#include <map>
#include <string>
#include <memory>

class Game {
public:
  ~Game();
  void initGame();
  void startGame();
  void loadMap();
  void drawMap();
  void endGame(const char*);
private:
  Knight* player_;
  std::map<Coords, Actor*> actors_;
  std::vector<std::vector<char>> map_;
};