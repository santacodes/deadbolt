#include <clip.h>
#include <clipboard/clipboard.hpp>

int Clipboard::set_clipboard(std::string clip_text) {
  bool res = clip::set_text(clip_text);
  if (!res)
    return 0;
  else
    return 1; // return 1 on error
}

int Clipboard::get_clipboard() {
  std::string value;
  bool res = clip::get_text(value);
  if (!res)
    return 0;
  else
    return 1; // return 1 on error
}
