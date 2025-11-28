// Flyweight is a structural design pattern that lets you fit more objects
// into the available amount of RAM by sharing common parts of state between
// multiple objects
//  instead of keeping all of the data in each object.
// Appicability:
// (*) when your program must support a huge number of objects which barely fit
// into available RAM,
//     and the objects contain duplicate states which can be extracted and
//     shared between multiple objects
// UML: docs/uml/patterns_structural_flyweight.drawio.svg

#include <iostream>
#include <string>
#include <unordered_map>

namespace {
namespace Problem {
class ImageContext {
 protected:
 private:
  // Shared attrs
  std::string fileName_;
  int width_, height_, dpi_;
  int* data_;

  // Unique attrs
  float scale_;
  float opacity_;

  void deepCopy(const ImageContext& other) {
    // normal field
    this->fileName_ = other.fileName_;
    this->width_ = other.width_;
    this->height_ = other.height_;
    this->dpi_ = other.dpi_;
    this->scale_ = other.scale_;
    this->opacity_ = other.opacity_;

    // ptr field
    delete[] this->data_;
    if (other.data_ != nullptr) {
      int size = this->width_ * this->height_;
      this->data_ = new int[size];
      for (int i = 0; i < size; ++i) {
        this->data_[i] = other.data_[i];
      }
    } else {
      this->data_ = nullptr;
    }
  }

 public:
  // Role of five
  // CONSTRUCTOR
  explicit ImageContext(const std::string& fileName, int w = 8, int h = 8,
                        int dpi = 96, float scale = 1, float opacity = 1)
      : fileName_{fileName},
        width_{w},
        height_{h},
        dpi_{dpi},
        data_{new int[w * h]},
        scale_{scale},
        opacity_{opacity} {
    std::cout << "Image: " << fileName_ << " created.\n";
  }

  // DESTRUCTOR
  virtual ~ImageContext() { delete[] data_; }

  // COPY CONSTRUCTOR
  ImageContext(const ImageContext& other) { this->deepCopy(other); }

  // COPY ASSIGNMENT
  ImageContext& operator=(const ImageContext& other) {
    if (this != &other) {
      this->deepCopy(other);
    }
    return *this;
  }

  void setScale(const float scale) { this->scale_ = scale; }

  void setOpacity(const float opacity) { this->opacity_ = opacity; }

  void display() const {
    std::cout << "Image: " << fileName_ << " displayed. \n";
    std::cout << "\tWidth: " << width_ << " Height: " << height_
              << " DPI: " << dpi_ << "\n";
    std::cout << "\tScale: " << scale_ << " Opacity: " << opacity_ << "\n";
  }
};

namespace Client {
void clientCode(const ImageContext* img) {
  img->display();
}
}  // namespace Client

void run() {
  std::cout << "\n\nProblem\n";
  ImageContext** imgs = new ImageContext*[5];
  for (int i = 0; i < 5; i++) {
    imgs[i] = new ImageContext("img1.svg");
    imgs[i]->setOpacity(1 - (float)1 / (i + 1));
    imgs[i]->setScale((float)1 / (i + 1));
    Client::clientCode(imgs[i]);
  }

  // delete objects first
  for (int i = 0; i < 5; i++) {
    delete imgs[i];
  }

  // then delete array
  delete[] imgs;
  std::cout << "Size of objects: " << sizeof(ImageContext) * 5 << "\n";
}
}  // namespace Problem

namespace FlyweightPattern {

/**
 * The Flyweight stores a common portion of the state (also called intrinsic
 * state) that belongs to multiple real business entities.
 * The Flyweight accepts the rest of the state (extrinsic state, unique for each
 * entity) via its method parameters.
 */
class ImageFlyweight {
 private:
  std::string fileName_;
  int width_, height_, dpi_;
  int* data_;

  void deepCopy(const ImageFlyweight& other) {
    // normal field
    this->fileName_ = other.fileName_;
    this->width_ = other.width_;
    this->height_ = other.height_;
    this->dpi_ = other.dpi_;

    // ptr field
    delete[] this->data_;
    if (other.data_ != nullptr) {
      int size = this->width_ * this->height_;
      this->data_ = new int[size];
      for (int i = 0; i < size; ++i) {
        this->data_[i] = other.data_[i];
      }
    } else {
      this->data_ = nullptr;
    }
  }

 public:
  explicit ImageFlyweight(const std::string& fileName, int w = 8, int h = 8,
                          int dpi = 96)
      : fileName_{fileName},
        width_{w},
        height_{h},
        dpi_{dpi},
        data_{new int[w * h]} {};

  ~ImageFlyweight() { delete[] data_; }

  // Note:
  // [Copy constructor/assignment]: creates a new copy(shallow or deep) of the
  // source object's data.
  //                        -> Allocates new memory and duplicates all elements.
  //                        -> Source object remains unchanged.
  //
  // [Move constructor/assignment[] ("steal"): transfers ownership of the
  // source's resources
  //                        -> No new memory allocation; destination uses the
  //                        same memory as source.
  //                        -> Source is reset to a safe empty state (nullptr,
  //                        0, etc.).
  //                        ==> Much faster for large objects since no deep copy
  //                        occurs.

  ImageFlyweight(const ImageFlyweight& other) { this->deepCopy(other); }

  ImageFlyweight& operator=(const ImageFlyweight& other) {
    if (this != &other) {
      this->deepCopy(other);
    }
    return *this;
  }

  ImageFlyweight(ImageFlyweight&& source) noexcept
      : fileName_(std::move(source.fileName_)),
        width_(source.width_),
        height_(source.height_),
        dpi_(source.dpi_),
        data_(source.data_) {
    source.width_ = 0;
    source.height_ = 0;
    source.dpi_ = 0;
    source.data_ = nullptr;
  }

  ImageFlyweight& operator=(ImageFlyweight&& source) noexcept {
    if (this != &source) {
      // stealSource
      // free current resource
      delete[] data_;

      // move resource from source
      fileName_ = std::move(source.fileName_);  // move string
      width_ = source.width_;
      height_ = source.height_;
      dpi_ = source.dpi_;
      data_ = source.data_;

      // reset source
      source.width_ = 0;
      source.height_ = 0;
      source.dpi_ = 0;
      source.data_ = nullptr;
    }
    return *this;
  }

  /**
   * Display the image at give scale, opacity
   */
  void display(const float& scale, const float& opacity) const {
    std::cout << "Image: " << fileName_ << " displayed. \n";
    std::cout << "\tWidth: " << width_ << " Height: " << height_
              << " DPI: " << dpi_ << "\n";
    std::cout << "\tScale: " << scale << " Opacity: " << opacity << "\n";
  }
};

/**
 * The Flyweight Factory manages a pool of existing flyweights.
 *  With the factory, clients don’t create flyweights directly. Instead, they
 * call the factory, passing it bits of the intrinsic state of the desired
 * flyweight. The factory looks over previously created flyweights and either
 * returns an existing one that matches search criteria or creates a new one if
 * nothing is found.
 */
class ImageFlyweightFactory {
 private:
  std::unordered_map<std::string, ImageFlyweight>
      m_imageFlyweights;  // unordered_map => need to role of five
  static std::string getKey(const std::string& name, int w, int h, int dpi) {
    return "key_" + name + std::to_string(w * h * dpi);
  }

 public:
  ImageFlyweight& getImangeFlyweight(const std::string& fileName, int w = 8,
                                     int h = 8, int dpi = 96) {
    std::string key = getKey(fileName, w, h, dpi);
    std::cout << "Key: " << key << "\n";
    if (this->m_imageFlyweights.find(key) == this->m_imageFlyweights.end()) {
      std::cout << "ImageFlyweightFactory: Can't find a image flyweight, "
                   "creating new one.\n";
      this->m_imageFlyweights.insert(
          std::make_pair(key, ImageFlyweight(fileName, w, h, dpi)));
    } else {
      std::cout << "ImageFlyweightFactory: Reusing existing image flyweight.\n";
    }
    return this->m_imageFlyweights.at(key);
  }
};

/**
 * The Context class contains the extrinsic state, unique across all original
 * objects. When a context is paired with one of the flyweight objects, it
 * represents the full state of the original object.
 */
class ImageContext {
 private:
  float scale_;
  float opacity_;
  ImageFlyweight* m_flyweight;

 public:
  ImageContext(ImageFlyweight& imageFlyweight, const float& s, const float& o)
      : scale_{s}, opacity_{o}, m_flyweight{&imageFlyweight} {}

  void display() const { this->m_flyweight->display(scale_, opacity_); }
};

/**
 * The Client calculates or stores the extrinsic state of flyweights.
 * From the client’s perspective, a flyweight is a template object which can be
 * configured at runtime by passing some contextual data into parameters of its
 * methods.
 */
namespace Client {
void clientCode(const ImageContext* img) {
  img->display();
}
}  // namespace Client

void run() {
  std::cout << "\n\nFlyweight\n";
  // Regis the images to the factory
  ImageFlyweightFactory* imageRegister = new ImageFlyweightFactory();
  ImageContext* img1 =
      new ImageContext(imageRegister->getImangeFlyweight("img1.svg"), 1, 0.1);
  Client::clientCode(img1);
  ImageContext* img2 =
      new ImageContext(imageRegister->getImangeFlyweight("img1.svg"), 1, 0.2);
  Client::clientCode(img2);
  ImageContext* img3 =
      new ImageContext(imageRegister->getImangeFlyweight("img1.svg"), 1, 0.3);
  Client::clientCode(img3);
  ImageContext* img4 =
      new ImageContext(imageRegister->getImangeFlyweight("img1.svg"), 1, 0.4);
  Client::clientCode(img4);
  ImageContext* img5 =
      new ImageContext(imageRegister->getImangeFlyweight("img1.svg"), 1, 0.5);
  Client::clientCode(img5);
  std::cout << "Size of objects: "
            << sizeof(ImageFlyweightFactory) + sizeof(ImageContext) * 5 << "\n";

  delete img1;
  delete img2;
  delete img3;
  delete img4;
  delete img5;
  delete imageRegister;
}

}  // namespace FlyweightPattern
}  // namespace

struct FlyweightAutoRunner {
  FlyweightAutoRunner() {
    std::cout << "\n--- Flyweight Pattern Example ---\n";
    Problem::run();
    FlyweightPattern::run();
  }
};

static FlyweightAutoRunner instance;