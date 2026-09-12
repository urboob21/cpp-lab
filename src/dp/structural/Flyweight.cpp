// -----------------------------------------------------------------------------
// Flyweight (structural pattern)
//
// Fits more objects into memory by sharing the parts of their state that are
// identical, instead of storing a copy in every object.
//
//   intrinsic state : shared, immutable (the pixels of an image, a font glyph)
//   extrinsic state : unique per object (position, scale, opacity)
//
// Problem: thousands of sprites of the same tree image each store their own
// copy of the pixel data.
// Solution: one shared, immutable ImageData per distinct image; each sprite
// only stores a pointer to it plus its own extrinsic state.
//
// Use it when:
//   - a program needs a huge number of similar objects
//   - most of their state can be extracted and shared
//
// Participants:
//   Flyweight         ImageData (intrinsic state, immutable)
//   FlyweightFactory  ImageCache - returns an existing flyweight or creates one
//   Context           Sprite (extrinsic state + pointer to the flyweight)
//
// UML: docs/uml/dp/structural_flyweight.drawio.svg
// -----------------------------------------------------------------------------

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

constexpr int kWidth = 32;
constexpr int kHeight = 32;
constexpr std::size_t kSpriteCount = 2'000;

namespace problem {

/// Every sprite owns its own copy of the pixels (Rule of Zero: std::vector copies itself).
struct Sprite {
  std::string file;
  std::vector<int> pixels;  // intrinsic, duplicated in every object
  float x;
  float y;
  float scale;
};

void run() {
  LOG_SECTION("Problem: every object stores the same pixels");
  std::vector<Sprite> forest;
  forest.reserve(kSpriteCount);
  for (std::size_t i = 0; i < kSpriteCount; ++i) {
    forest.push_back({"tree.png", std::vector<int>(kWidth * kHeight),
                      static_cast<float>(i), 0.0F, 1.0F});
  }
  const std::size_t bytes =
      forest.size() * (sizeof(Sprite) + kWidth * kHeight * sizeof(int));
  LOG_S("  " << forest.size() << " sprites use about " << bytes / 1024
             << " KiB");
}

}  // namespace problem

namespace flyweight {

/// Flyweight: intrinsic state, immutable and therefore safe to share.
class ImageData {
 public:
  ImageData(std::string file, int width, int height)
      : file_{std::move(file)},
        width_{width},
        height_{height},
        pixels_(width * height) {
    LOG_S("    loading " << file_ << " (" << width_ << "x" << height_ << ")");
  }

  const std::string& file() const { return file_; }
  std::size_t bytes() const {
    return sizeof(*this) + pixels_.size() * sizeof(int);
  }

 private:
  std::string file_;
  int width_;
  int height_;
  std::vector<int> pixels_;
};

/// Flyweight factory: one ImageData per distinct (file, width, height).
class ImageCache {
 public:
  std::shared_ptr<const ImageData> get(const std::string& file, int width,
                                       int height) {
    const auto key =
        std::make_tuple(file, width, height);  // all intrinsic fields
    auto& slot = cache_[key];
    if (!slot) {
      slot = std::make_shared<const ImageData>(file, width, height);
    }
    return slot;
  }

  std::size_t size() const { return cache_.size(); }
  std::size_t bytes() const {
    std::size_t total = 0;
    for (const auto& [key, image] : cache_) {
      total += image->bytes();
    }
    return total;
  }

 private:
  std::map<std::tuple<std::string, int, int>, std::shared_ptr<const ImageData>>
      cache_;
};

/// Context: extrinsic state + shared flyweight.
struct Sprite {
  std::shared_ptr<const ImageData> image;
  float x;
  float y;
  float scale;
};

void run() {
  LOG_SECTION("Flyweight: share the pixels, keep only positions per object");
  ImageCache cache;
  std::vector<Sprite> forest;
  forest.reserve(kSpriteCount);
  for (std::size_t i = 0; i < kSpriteCount; ++i) {
    const char* file =
        (i % 3 == 0) ? "pine.png" : "oak.png";  // two distinct images
    forest.push_back(
        {cache.get(file, kWidth, kHeight), static_cast<float>(i), 0.0F, 1.0F});
  }

  const std::size_t bytes = forest.size() * sizeof(Sprite) + cache.bytes();
  LOG_S("  " << forest.size() << " sprites share " << cache.size()
             << " images, about " << bytes / 1024 << " KiB in total");
  LOG_S("  sprite 0 draws " << forest[0].image->file() << ", sprite 1 draws "
                            << forest[1].image->file() << " (use_count "
                            << forest[1].image.use_count() << ")");
}

}  // namespace flyweight

}  // namespace

LAB_EXAMPLE("Flyweight",
            "share immutable intrinsic state between many small objects") {
  problem::run();
  flyweight::run();
}
