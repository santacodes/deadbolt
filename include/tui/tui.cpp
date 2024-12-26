#include "tui.hpp"
#include <ftxui/component/component.hpp>   // For Renderer, Button, Input, etc.
#include <ftxui/component/screen_interactive.hpp>  // For ScreenInteractive
#include <ftxui/dom/elements.hpp>  // For text, hbox, vbox, border
#include <iostream>

using namespace ftxui;

int Tui::tui() {
  // Example credentials
  std::vector<std::pair<std::string, std::string>> credentials = {
      {"Email", "123456"},
      {"GitHub", "123456"},
      {"AWS", "123456"},
      {"Bank", "123456"},
  };

  // State variables for new entry
  std::string new_service, new_key;

  // TOTP Display Section
  auto totp_entries = Container::Vertical({});
  auto totp_display = Renderer(totp_entries, [&] {
    Elements rows;
    for (const auto& credential : credentials) {
      rows.push_back(
          hbox({
              text(credential.first) | bold | color(Color::Blue),
              filler(),
              text(credential.second) | bold | color(Color::Green),
          }) | border);
    }
    return vbox(std::move(rows)) | border | size(HEIGHT, LESS_THAN, 20);
  });

  // New Entry Input Section
  auto service_input = Input(&new_service, "Service Name");
  auto key_input = Input(&new_key, "Key");
  auto add_button = Button("Add Entry", [&] {
    if (!new_service.empty() && !new_key.empty()) {
      std::cout << new_service << new_key <<std::endl;
      credentials.emplace_back(new_service, new_key);
      new_service.clear();
      new_key.clear();
    }
  });
  auto new_entry_box = Container::Vertical({
      service_input,
      key_input,
      add_button,
  });
  auto new_entry_renderer = Renderer(new_entry_box, [&] {
    return vbox({
               text("Add New Entry:") | bold | color(Color::Yellow),
               hbox(text("Service: "), service_input->Render()) | border,
               hbox(text("Key: "), key_input->Render()) | border,
               add_button->Render() | center,
           }) |
           border;
  });

  // Export Section
  auto export_button = Button("Export Keys", [&] {
    // Placeholder for export functionality
    std::cout << "exporting keys";
  });
  auto export_box = Renderer(export_button, [&] {
    return vbox({
               text("Export Keys:") | bold | color(Color::Yellow),
               export_button->Render() | center,
           }) |
           border;
  });

  // Main Layout
  auto main_container = Container::Horizontal({
      totp_entries,
      new_entry_box,
      export_button,
  });

  auto main_renderer = Renderer(main_container, [&] {
    return hbox({
        totp_display->Render(),
        new_entry_renderer->Render(),
        export_box->Render(),
    });
  });

  // Screen setup
  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(main_renderer);

  return 0;
}

