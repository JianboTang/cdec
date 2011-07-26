#ifndef _FDICT_H_
#define _FDICT_H_

#include <string>
#include <vector>
#include "dict.h"

struct FD {
  // once the FD is frozen, new features not already in the
  // dictionary will return 0
  static void Freeze() {
    frozen_ = true;
  }
  static void UnFreeze() {
    frozen_ = false;
  }

  static inline int NumFeats() {
    return dict_.max() + 1;
  }
  static inline WordID Convert(const std::string& s) {
    return dict_.Convert(s, frozen_);
  }
  static inline const std::string& Convert(const WordID& w) {
    return dict_.Convert(w);
  }
  static std::string Convert(WordID const *i,WordID const* e);
  static std::string Convert(std::vector<WordID> const& v);

  // Escape any string to a form that can be used as the name
  // of a weight in a weights file
  static std::string Escape(const std::string& s);
  static Dict dict_;
 private:
  static bool frozen_;
};

#endif
