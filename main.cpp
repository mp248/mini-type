#include <chrono>
#include <fstream>
#include <ncurses.h>
#include <random>
#include <string>

using namespace std;

// Renders the characters to the left of the cursor.
void render_left(const string& typed, const string& words, int cursor_x, int cursor_y) {
  for (size_t i = 0; i < typed.size(); i++) {
    int x_pos = cursor_x - (typed.size() - i);
    if (typed[i] == words[i]) attron(COLOR_PAIR(1));
    else attron(COLOR_PAIR(2));
    mvaddch(cursor_y, x_pos, typed[i]);
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
  }
}

// Renders the characters to the right of the cursor.
void render_right(const string& typed, const string& words, int cursor_x, int cursor_y) {
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
}

int main() {
  initscr();
  start_color();
  use_default_colors();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  // Color pairs.
  init_pair(1, COLOR_GREEN, -1);          // Correctly typed characters.
  init_pair(2, COLOR_RED, -1);            // Incorrectly typed characters.
  init_pair(3, COLOR_BLACK, COLOR_WHITE); // "Cursor"

  // Cursor position.
  int cursor_x = COLS / 4;
  int cursor_y = LINES / 2;

  // Characters to the left of the cursor.
  string typed = "";

  // Characters to the right of the cursor.
  string words = "";

  // Create word bank.
  vector<string> word_bank;
  ifstream input_file("words.txt");
  string line;
  while (getline(input_file, line)) word_bank.push_back(line + ' ');
  input_file.close();

  // Generate test.
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  mt19937_64 engine(seed);
  uniform_int_distribution<int> distribution(0, word_bank.size() - 1);
  for (int i = 0; i < 100; i++) {
    int random_index = distribution(engine);
    words += word_bank[random_index];
  }

  // Display test on launch.
  render_right(typed, words, cursor_x, cursor_y);

  while (true) {
    // Read input from keyboard.
    int in = getch();
    if (in == 27) break;
    if (('a' <= (char)in && (char)in <= 'z') || (char)in == ' ' || in == 127) {
      if (in == 127) { if (!typed.empty()) typed.erase(typed.size() - 1, 1);
      } else typed += in;

      // Render the test.
      clear();
      render_left(typed, words, cursor_x, cursor_y);
      render_right(typed, words, cursor_x, cursor_y);
      refresh();

      // Replace each typed word with a new random word.
      if (words[typed.size()] == ' ') {
        int random_index = distribution(engine);
        words += word_bank[random_index];
      }
    }
  }

  endwin();

  return 0;
}
