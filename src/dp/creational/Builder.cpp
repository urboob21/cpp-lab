// -----------------------------------------------------------------------------
// Builder (creational pattern)
//
// Constructs a complex object step by step. The same construction process can
// create different representations.
//
// Problem: a constructor with many (often optional) parameters -
//   House(walls, doors, windows, roof, garage, pool, garden, ...)
// is hard to read and easy to call with arguments in the wrong order.
//
// Participants:
//   Product          House
//   Builder          HouseBuilder interface: one method per step
//   ConcreteBuilder  WoodenHouseBuilder, StoneHouseBuilder
//   Director         defines the ORDER of steps for common configurations
//
// The second part shows the "fluent builder" that is common in C++ code:
// chained setters and a build() that validates the result.
//
// UML: docs/uml/dp/creational_builder.drawio.svg
// -----------------------------------------------------------------------------

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace classic {

/// Product.
class House {
 public:
  void addPart(std::string part) { parts_.push_back(std::move(part)); }

  std::string describe() const {
    std::string text;
    for (const std::string& part : parts_) {
      text += (text.empty() ? "" : ", ") + part;
    }
    return text.empty() ? "(empty)" : text;
  }

 private:
  std::vector<std::string> parts_;
};

/// Builder interface.
class HouseBuilder {
 public:
  virtual ~HouseBuilder() = default;
  virtual void buildWalls() = 0;
  virtual void buildRoof() = 0;
  virtual void buildGarage() = 0;
  virtual void buildPool() = 0;

  /// Hands out the finished product and starts a fresh one, so the builder
  /// can be reused safely.
  std::unique_ptr<House> result() {
    auto finished = std::move(house_);
    house_ = std::make_unique<House>();
    return finished;
  }

 protected:
  House& house() { return *house_; }

 private:
  std::unique_ptr<House> house_ = std::make_unique<House>();
};

class WoodenHouseBuilder : public HouseBuilder {
 public:
  void buildWalls() override { house().addPart("wooden walls"); }
  void buildRoof() override { house().addPart("shingle roof"); }
  void buildGarage() override { house().addPart("wooden carport"); }
  void buildPool() override { house().addPart("above-ground pool"); }
};

class StoneHouseBuilder : public HouseBuilder {
 public:
  void buildWalls() override { house().addPart("stone walls"); }
  void buildRoof() override { house().addPart("clay tile roof"); }
  void buildGarage() override { house().addPart("stone garage"); }
  void buildPool() override { house().addPart("in-ground pool"); }
};

/// Director: knows the recipes, not the materials.
class Director {
 public:
  static void buildMinimal(HouseBuilder& builder) {
    builder.buildWalls();
    builder.buildRoof();
  }

  static void buildLuxury(HouseBuilder& builder) {
    builder.buildWalls();
    builder.buildRoof();
    builder.buildGarage();
    builder.buildPool();
  }
};

void run() {
  LOG_SECTION("Director + builders: same steps, different representations");
  WoodenHouseBuilder wood;
  StoneHouseBuilder stone;

  Director::buildMinimal(wood);
  LOG_S("  minimal wooden house: " << wood.result()->describe());

  Director::buildLuxury(wood);  // the builder was reset by result()
  LOG_S("  luxury wooden house : " << wood.result()->describe());

  Director::buildLuxury(stone);
  LOG_S("  luxury stone house  : " << stone.result()->describe());

  stone.buildWalls();  // clients can also drive a builder without a director
  stone.buildPool();
  LOG_S("  custom stone house  : " << stone.result()->describe());
}

}  // namespace classic

namespace fluent {

class HttpRequest {
 public:
  class Builder;

  std::string describe() const {
    std::string text = method_ + " " + url_;
    for (const auto& [name, value] : headers_) {
      text += "\n      " + name + ": " + value;
    }
    if (!body_.empty()) {
      text += "\n      body: " + body_;
    }
    return text;
  }

 private:
  HttpRequest() = default;  // only the Builder can create requests
  std::string method_ = "GET";
  std::string url_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

class HttpRequest::Builder {
 public:
  explicit Builder(std::string url) { request_.url_ = std::move(url); }

  // Each setter returns *this, so calls can be chained.
  Builder& method(std::string method) {
    request_.method_ = std::move(method);
    return *this;
  }
  Builder& header(std::string name, std::string value) {
    request_.headers_[std::move(name)] = std::move(value);
    return *this;
  }
  Builder& body(std::string body) {
    request_.body_ = std::move(body);
    return *this;
  }

  /// Validates before handing out the product.
  HttpRequest build() const {
    if (request_.url_.rfind("http", 0) != 0) {
      throw std::invalid_argument("URL must start with http: " + request_.url_);
    }
    if (request_.method_ == "GET" && !request_.body_.empty()) {
      throw std::invalid_argument("a GET request must not have a body");
    }
    return request_;
  }

 private:
  HttpRequest request_;
};

void run() {
  LOG_SECTION("Fluent builder with validation");
  const HttpRequest request =
      HttpRequest::Builder("https://api.example.com/users")
          .method("POST")
          .header("Content-Type", "application/json")
          .header("Authorization", "Bearer token")
          .body(R"({"name": "Ada"})")
          .build();
  LOG_S("  " << request.describe());

  try {
    HttpRequest::Builder("https://example.com").body("oops").build();
  } catch (const std::invalid_argument& e) {
    LOG_S("  build() rejected an invalid request: " << e.what());
  }
}

}  // namespace fluent

}  // namespace

LAB_EXAMPLE("Builder",
            "step-by-step construction with builders, a director and a fluent "
            "builder") {
  classic::run();
  fluent::run();
}
