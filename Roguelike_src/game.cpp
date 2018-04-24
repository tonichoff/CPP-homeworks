#include "game.h"
#include "curses.h"
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

Game::~Game() {
  delete player_;
  for (auto it = actors_.begin(); it != actors_.end(); it++)
    delete it->second;

}

void Game::initGame() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, true);

  int row, col;
  getmaxyx(stdscr, row, col);

  const char* welcome_msg = "Roguelike";
  mvprintw(row / 2, (col - strlen(welcome_msg)) / 2, "%s", welcome_msg);

  const char* start_msg = "Press 'S' to start";
  mvprintw(row - 2, 0, "%s", start_msg);

  const char* exit_msg = "Press 'Q' to quit";
  mvprintw(row - 1, 0, "%s", exit_msg);

  while (true) {
    int command = getch();
    if (command == 'S' || command == 's') {
      startGame();
      endwin();
      break;
    }
    else if (command == 'Q' || command == 'q') {
      endwin();
      break;
    }
  }
}

void Game::startGame() {
  loadMap();
  drawMap();

  int command;
  while (command = getch()) {
    int up = -1 * (command == KEY_UP);
    int down = (command == KEY_DOWN);
    int left = -1 * (command == KEY_LEFT);
    int right = (command == KEY_RIGHT);
    
    Coords old_coords = player_->getCoords();
    Coords new_coords(old_coords.x_ + left + right, old_coords.y_ + up + down);
    bool valid_x = (new_coords.x_ >= 0) && (new_coords.x_ < map_[0].size());
    bool valid_y = (new_coords.y_ >= 0) && (new_coords.y_ < map_.size());
    if (valid_x && valid_y) {
      auto it = actors_.find(new_coords);
      if (it != actors_.end()) {
        player_->collide(it->second);
        if (player_->isWon()) {
          endGame("You won");
          break;
        }
        if (player_->isDead()) {
          endGame("You lose");
          break;
        }
        if (it->second->isDead()) {
          actors_.erase(new_coords);
          map_[old_coords.y_][old_coords.x_] = '.';
          map_[new_coords.y_][new_coords.x_] = 'K';
        }
      }
      else {
        player_->setCoords(new_coords);
        map_[old_coords.y_][old_coords.x_] = '.';
        map_[new_coords.y_][new_coords.x_] = 'K';
      }
    }
    drawMap();
  }

}

void Game::loadMap() {
  std::ifstream file_in("map.txt");

  while (!file_in.eof()) {
    std::string new_str;
    file_in >> new_str;
    map_.push_back(std::vector<char>());
    for (int j = 0; j < new_str.size(); j++) {
      int i = map_.size() - 1;
      map_[i].push_back(new_str[j]);
      Coords coords(j, i);
      if (map_[i][j] == 'K') 
        player_ = new Knight(coords, 'K');
      else if (map_[i][j] == '#') {
        Actor* wall = new Wall(coords, '#');
        actors_[coords] = wall;
      }
      else if (map_[i][j] == 'Z') {
        Actor* zombie = new Zombie(coords, 'Z');
        actors_[coords] = zombie;
      }
      else if (map_[i][j] == 'P') {
        Actor* princess = new Princess(coords, 'P');
        actors_[coords] = princess;
      }
    }
  }
}

void Game::drawMap() {
  clear();
  int row, col;
  getmaxyx(stdscr, row, col);

  for (int i = 0; i < map_.size(); i++) {
    for (int j = 0; j < map_[i].size(); j++) {
      mvaddch(i, j, '.');
      auto it = actors_.find(Coords(j, i));
      if (it != actors_.end())
        mvaddch(i, j, it->second->getSymbol());
    }
  }
  mvaddch(player_->getCoords().y_, player_->getCoords().x_, player_->getSymbol());
}

void Game::endGame(const char *result_msg) {
  int row, col;
  getmaxyx(stdscr, row, col);

  clear();
  mvprintw(row / 2, (col - strlen(result_msg)) / 2, "%s", result_msg);
  const char* exit_msg = "Press any key to quit";
  mvprintw(row - 1, 0, "%s", exit_msg);

  getch();
}