#include <fstream>
#include <ncurses.h>
#include <string>

using namespace std;

int main() {
  initscr();
  start_color();
  use_default_colors();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  // Color pairs.
  init_pair(1, COLOR_GREEN, -1);
  init_pair(2, COLOR_RED, -1);
  init_pair(3, COLOR_BLACK, COLOR_WHITE);

  // Cursor position.
  int cursor_x = COLS / 4;
  int cursor_y = LINES / 2;

  // What has been typed.
  string typed = "";

  // Generate and display test on launch.
  string words = "";
  ifstream input_file("words.txt");
  string word;
  for (int i = 0; i < 100; i++) {
    getline(input_file, word);
    words += word + ' ';
  }
  input_file.close();

  size_t pos = typed.size();
  for (size_t i = 0; i < words.size(); i++) {
    int x_pos = cursor_x + i;
    if (x_pos >= COLS) break;
    if (i == 0) {
      attron(COLOR_PAIR(3));
      mvaddch(cursor_y, x_pos, words[pos + i]);
      attroff(COLOR_PAIR(3));
    } else mvaddch(cursor_y, x_pos, words[pos + i]);
  }

  while (true) {
    // Read input from keyboard.
    int ch = getch();
    if (ch == 27) break;
    if (('a' <= (char)ch && (char)ch <= 'z') || (char)ch == ' ') {
      // Append input to string of typed characters.
      typed += ch;

      // Render the test.
      clear();

      for (size_t i = 0; i < typed.size(); i++) {
        int x_pos = cursor_x - (typed.size() - i);
        if (typed[i] == words[i]) attron(COLOR_PAIR(1));
        else attron(COLOR_PAIR(2));
        mvaddch(cursor_y, x_pos, typed[i]);
        attroff(COLOR_PAIR(1));
        attroff(COLOR_PAIR(2));
      }

      size_t pos = typed.size();
      for (size_t i = 0; i < words.size(); i++) {
        int x_pos = cursor_x + i;
        if (x_pos >= COLS) break;
        if (i == 0) {
          attron(COLOR_PAIR(3));
          mvaddch(cursor_y, x_pos, words[pos + i]);
          attroff(COLOR_PAIR(3));
        } else mvaddch(cursor_y, x_pos, words[pos + i]);
      }

      refresh();
    }
  }

  endwin();
  return 0;
}
