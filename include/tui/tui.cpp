#include "tui.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <vector>

using namespace ftxui;

int Tui::tui() {
  // Example credentials
  std::vector<std::pair<std::string, std::string>> credentials = {
      {"Email", "123456"},
      {"GitHub", "123456"},
      {"AWS", "123456"},
      {"Bank", "123456"},
  };

  // Main FTXUI renderer
  auto renderer = Renderer([&] {
    Elements rows;
    for (size_t i = 0; i < credentials.size(); ++i) {
      rows.push_back(
          hbox({
              text(credentials[i].first) | bold | color(Color::Blue),
              filler(),
              text(credentials[i].second) | bold | color(Color::Green),
          }) |
          border);
    }
    return vbox(std::move(rows)) | border | size(HEIGHT, LESS_THAN, 20);
  });

  // Screen
  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(renderer);

  return 0;
}
