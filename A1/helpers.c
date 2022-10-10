/*
 * DO NOT CHANGE THIS FILE!
 */

#include "spacecollector.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void init_map()
{
  for (int y = 0; y < MAP_LENGTH; y++)
  {
    for (int x = 0; x < MAP_WIDTH; x++)
    {
      game_map[y][x] = (char)EMPTY;
    }
  }
  initscr();

  start_color();
  init_pair(SPACE, COLOR_YELLOW, COLOR_BLACK);
  init_pair(PLAYER_COLOR, COLOR_WHITE, COLOR_BLUE);
  init_pair(ALIEN_COLOR, COLOR_BLACK, COLOR_RED);
  init_pair(CRATE_COLOR, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(BLACKHOLE_COLOR, COLOR_WHITE, COLOR_BLACK);

  if (has_colors() != TRUE)
  {
    printf("ERROR: Your Screen does not support colors\n");
  }
  game_window = newwin(MAP_LENGTH, MAP_WIDTH, 0, 0);
}

char getMapValue(int x_pos, int y_pos)
{
  return game_map[y_pos][x_pos];
}

void spawnEnemy(char type, int x_pos, int y_pos)
{
  if (type == ENEMY_BLACKHOLE)
  {
    game_map[y_pos][x_pos] = (char)ENEMY_BLACKHOLE;
  }
  else if (type == ENEMY_ALIEN)
  {
    game_map[y_pos][x_pos] = (char)ENEMY_ALIEN;
  }
}

void spawnCrate(int x_pos, int y_pos)
{
  game_map[y_pos][x_pos] = (char)SUPPLY_CRATE;
}

void refreshMap()
{
  werase(game_window);

  mvwprintw(game_window, 1, 1, "SPACECOLLECTOR");
  mvwprintw(game_window, 1, 43, "v.0.1.0");
  mvwprintw(game_window, 2, 1, "POINTS: %07d\n", points);

  for (int y = 3; y < MAP_LENGTH; ++y)
  {
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
      if (game_map[y][x] != EMPTY)
      {
        if (game_map[y][x] == PLAYER)
        {
          wattron(game_window, COLOR_PAIR(PLAYER_COLOR));
          mvwaddch(game_window, y, x, 'P');
          wattroff(game_window, COLOR_PAIR(PLAYER_COLOR));
        }
        else if (game_map[y][x] == ENEMY_ALIEN)
        {
          wattron(game_window, COLOR_PAIR(ALIEN_COLOR));
          mvwaddch(game_window, y, x, 'A');
          wattroff(game_window, COLOR_PAIR(ALIEN_COLOR));
        }
        else if (game_map[y][x] == SUPPLY_CRATE)
        {
          wattron(game_window, COLOR_PAIR(CRATE_COLOR));
          mvwaddch(game_window, y, x, '#');
          wattroff(game_window, COLOR_PAIR(CRATE_COLOR));
        }
        else if (game_map[y][x] == ENEMY_BLACKHOLE)
        {
          wattron(game_window, COLOR_PAIR(BLACKHOLE_COLOR));
          mvwaddch(game_window, y, x, 'B');
          wattroff(game_window, COLOR_PAIR(BLACKHOLE_COLOR));
        }
      }
      else
      {
        wattron(game_window, COLOR_PAIR(SPACE));
        mvwaddch(game_window, y, x, '\0');
        wattroff(game_window, COLOR_PAIR(SPACE));
      }
    }
  }
  wrefresh(game_window);
}

char getRandPosX()
{
  return rand() % 30 + 5;
}

char getRandPosY()
{
  return rand() % 30 + 5;
}

void movePlayer(char *direction, char *prev_direction, position *pos, char type)
{
  switch (*direction)
  {
  case 'l':
    if (pos->x_ < 1)
    {
      if (type == PLAYER) 
      {
        lifepoints = 0;
        break;
      }
      else
      {
        // removing person
        game_map[pos->y_][pos->x_] = (char)EMPTY;

        // revoking on the opposite site of the map
        pos->x_ = MAP_WIDTH - 1;
        game_map[pos->y_][pos->x_] = (char)type;

        break;
      }
    }
    if (type == PLAYER)
    {
      if (game_map[pos->y_][pos->x_ - 1] == (char)ENEMY_BLACKHOLE ||
          game_map[pos->y_][pos->x_ - 1] == (char)ENEMY_ALIEN)
      {
        lifepoints = 0;
        break;
      }
    }
    game_map[pos->y_][pos->x_] = (char)EMPTY;
    game_map[pos->y_][--pos->x_] = (char)type;
    break;

  case 'r':
    if (pos->x_ > MAP_WIDTH - 2)
    {
      if (type == PLAYER) 
      {
        lifepoints = 0;
        break;
      }
      else
      {
        // removing person
        game_map[pos->y_][pos->x_] = (char)EMPTY;

        // revoking on the opposite site of the map
        pos->x_ = 0;
        game_map[pos->y_][pos->x_] = (char)type;

        break;
      }
    }

    if (type == PLAYER)
    {
      if (game_map[pos->y_][pos->x_ + 1] == (char)ENEMY_BLACKHOLE ||
          game_map[pos->y_][pos->x_ + 1] == (char)ENEMY_ALIEN)
      {
        lifepoints = 0;
        break;
      }
    }
    game_map[pos->y_][pos->x_] = (char)EMPTY;
    game_map[pos->y_][++pos->x_] = (char)type;
    break;

  case 'u':
    if (pos->y_ < 4)
    {
      if (type == PLAYER) 
      {
        lifepoints = 0;
        break;
      }
      else
      {
        // removing person
        game_map[pos->y_][pos->x_] = (char)EMPTY;

        // revoking on the opposite site of the map
        pos->y_ = MAP_LENGTH - 3;
        game_map[pos->y_][pos->x_] = (char)type;

        break;
      }
    }

    if (type == PLAYER)
    {
      if (game_map[pos->y_ - 1][pos->x_] == (char)ENEMY_BLACKHOLE ||
          game_map[pos->y_ - 1][pos->x_] == (char)ENEMY_ALIEN)
      {
        lifepoints = 0;
        break;
      }
    }
    game_map[pos->y_][pos->x_] = (char)EMPTY;
    game_map[--pos->y_][pos->x_] = (char)type;
    break;

  case 'd':
    if (pos->y_ > MAP_LENGTH - 2)
    {
      if (type == PLAYER) 
      {
        lifepoints = 0;
        break;
      }
      else
      {
        // removing person
        game_map[pos->y_][pos->x_] = (char)EMPTY;

        // revoking on the opposite site of the map
        pos->y_ = 3;
        game_map[pos->y_][pos->x_] = (char)type;

        break;
      }
    }

    if (type == PLAYER)
    {
      if (game_map[pos->y_ + 1][pos->x_] == (char)ENEMY_BLACKHOLE ||
          game_map[pos->y_ + 1][pos->x_] == (char)ENEMY_ALIEN)
      {
        lifepoints = 0;
        break;
      }
    }
    game_map[pos->y_][pos->x_] = (char)EMPTY;
    game_map[++pos->y_][pos->x_] = (char)type;
    break;

  default:
    break;
  }
  *prev_direction = *direction;
}

int end_game(void *rvalue, void **rvalues_enemies, void *rvalue_crate)
{
  mvwprintw(game_window, (MAP_LENGTH / 2), 16, "    GAME OVER    ");
  mvwprintw(game_window, (MAP_LENGTH / 2 + 1), 16, " %08d POINTS ", points);
  wrefresh(game_window);
  usleep(5000000);
  wborder(game_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wclear(game_window);
  wrefresh(game_window);
  delwin(game_window);
  endwin();

  return 0;
}
