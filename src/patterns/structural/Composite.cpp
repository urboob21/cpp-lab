// Composite is a structural design pattern
// that lets you compose objects into "tree structures"
// and then work with these structures as if they were individual objects.
// Appicability:
// (*)   when you have to implement a tree-like object structure.
// (**)  when you want the client code to treat both simple and complex elements
// uniformly. UML: docs/uml/patterns_structural_composite.drawio.svg

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

namespace {
namespace Problem {
class File {
 private:
  std::string _name;

 public:
  explicit File(const std::string& fileName) : _name{fileName} {}
  std::string getName() const { return this->_name; }

  void setName(const std::string& name) { this->_name = name; }

  void open() const { std::cout << "Open file: " << _name << "\n"; }
};

// [P1] Have the NewTypeFile => Update Folder (fields, functions)

class Folder {
 private:
  std::string _name;
  std::list<File*>
      _files;  // Should store the pointer to the actual file so when we delete
               // the file it should delete in the list
  std::list<Folder*> _subFolders;
  // [P2] What about the parrent ?

 public:
  explicit Folder(const std::string& name) : _name{name} {}

  ~Folder() {
    for (File* f : _files) {
      delete f;
    }

    for (Folder* sf : _subFolders) {
      delete sf;
    }
  }

  void removeFile(const File* file) {
    _files.remove_if([file](const File* f) { return f == file; });
  }

  void removeFileByName(const std::string& name) {

    // for (auto i = _files.begin(); i != _files.end(); ++i)
    // {
    //     if ((*i)->getName() == name)
    //     {
    //         delete (*i);
    //         _files.erase(i);
    //         return;
    //     }
    // }

    auto it =
        std::find_if(_files.begin(), _files.end(),
                     [&name](const File* f) { return f->getName() == name; });

    if (it != _files.end()) {
      delete *it;        // free the memory
      _files.erase(it);  // remove the pointer from the list
    }
  }

  void removeFolderByName(const std::string& name) {
    // for (auto i = _subFolders.begin(); i != _subFolders.end(); ++i)
    // {
    //     if ((*i)->getName() == name)
    //     {
    //         delete (*i);
    //         _subFolders.erase(i);
    //         return;
    //     }
    // }
    auto it =
        std::find_if(_subFolders.begin(), _subFolders.end(),
                     [&name](const Folder* f) { return f->getName() == name; });

    if (it != _subFolders.end()) {
      delete *it;             // free the memory
      _subFolders.erase(it);  // remove the pointer from the list
    }
  }

  void addFile(File* file) { _files.push_back(file); }

  void addFolder(Folder* folder) { _subFolders.push_back(folder); }

  void removeFolder(const Folder* folder) {
    _subFolders.remove_if([folder](const Folder* f) { return f == folder; });
  }

  std::string getName() const { return _name; }

  void open() const { std::cout << "Open Folder: " << _name << "\n"; }

  int size() const {
    int size = static_cast<int>(_files.size());

    //  Consider using std::accumulate algorithm instead of a raw loop.
    std::for_each(_subFolders.begin(), _subFolders.end(),
                  [&size](const Folder* sf) { size += sf->size(); });

    // for (const Folder *subFolder : _subFolders)
    // {
    //     size += subFolder->size();
    // }

    return size;
  }

  const std::list<Folder*>& getSubFolders() const { return _subFolders; }

  const std::list<File*>& getFiles() const { return _files; }

  void getFilesRecursive(std::list<File*>& out) const {
    out.insert(out.end(), _files.begin(), _files.end());
    for (const Folder* sf : _subFolders) {
      sf->getFilesRecursive(out);
    }
  }

  void getSubFoldersRecursive(std::list<Folder*>& out) const {
    out.insert(out.end(), _subFolders.begin(), _subFolders.end());
    for (const Folder* sf : _subFolders) {
      sf->getSubFoldersRecursive(out);
    }
  }
};

namespace Client {
void clientCode(const Folder* folder) {
  std::cout << "File name: " << folder->getName() << "\n";
  folder->open();

  std::cout << "Folder size: " << folder->getName()
            << ", size: " << folder->size() << "\n";
  std::list<File*> files;
  folder->getFilesRecursive(files);
  for (const File* f : files) {
    std::cout << "\t\tFile: " << f->getName() << "\n";
  }

  std::list<Folder*> subFolders;
  folder->getSubFoldersRecursive(subFolders);
  for (const Folder* sf : subFolders) {
    std::cout << "\t\tFolder: " << sf->getName() << "\n";
  }
}
}  // namespace Client

void run() {
  std::cout << "\n\n";
  Folder* root = new Folder("root");
  root->open();

  // Prepare files
  File* file1 = new File("fileName1.txt");
  File* file2 = new File("fileName2.txt");
  File* file3 = new File("fileName3.txt");
  root->addFile(new File(*file1));
  root->addFile(new File(*file2));
  root->addFile(new File(*file3));

  Folder* subFolder1 = new Folder("subFolder1");
  Folder* subFolder2 = new Folder("subFolder2");
  Folder* subFolder3 = new Folder("subFolder3");

  root->addFolder(new Folder(*subFolder1));
  root->addFolder(new Folder(*subFolder2));
  root->addFolder(new Folder(*subFolder3));

  Client::clientCode(root);

  // Clean up memory
  delete subFolder1;
  delete subFolder2;
  delete subFolder3;
  delete file1;
  delete file2;
  delete file3;

  root->removeFileByName("fileName1.txt");
  Client::clientCode(root);
  delete root;  // deletes all files/subfolders inside recursively
}

}  // namespace Problem

namespace CompositePattern {
/**
 * Component is the abstraction for leafs and composites.
 * It defines the interface that must be implemented by the objects in the
 * composition. For example a file system resource defines move, copy, rename,
 * and getSize methods for files and folders.
 */
class FileSystem {
 private:
  FileSystem* _parent;
  std::string _name;

 public:
  explicit FileSystem(const std::string& fileName)
      : _parent{nullptr}, _name{fileName} {}
  virtual ~FileSystem() {
    std::cout << "Destructor: " << this->getName() << "\n";
  }

  FileSystem* getParent() const { return _parent; }

  void setParent(FileSystem* parent) { _parent = parent; }

  std::string getName() const { return this->_name; }

  virtual int size() const { return 1; }

  void setName(const std::string& name) { this->_name = name; }

  virtual void add(FileSystem* fs) {
    // do nothing here
  }

  virtual void remove(FileSystem* fs) {
    // do nothing here
  }

  /**
   * Check if the component is composite or not
   * You can provide a method that lets the client code figure out whether a
   * component can bear children.
   */
  virtual bool isComposite() const { return false; }

  virtual void open() const = 0;
};

/**
 * Composite - A Composite stores child components in addition to implementing
 * methods defined by the component interface. Composites implement methods
 * defined in the Component interface by delegating to child components. In
 * addition composites provide additional methods for adding, removing, as well
 * as getting components.
 */
class Folder : public FileSystem {
 private:
  std::list<FileSystem*> _children;

  // Remove children
  [[maybe_unused]] void removeChildren(const FileSystem* file) {
    _children.remove_if([file](const FileSystem* f) { return f == file; });
  }

  [[maybe_unused]] void removeChildrenByName(const std::string& name) {

    auto it = std::find_if(
        _children.begin(), _children.end(),
        [&name](const FileSystem* f) { return f->getName() == name; });

    if (it != _children.end()) {
      delete *it;           // free the memory
      _children.erase(it);  // remove the pointer from the list
    }
  }

  [[maybe_unused]] const std::list<FileSystem*>& getChildren() const {
    return _children;
  }

  void getChildrensRecursive(std::list<FileSystem*>& out) const {
    for (FileSystem* fs : _children) {
      if (fs->isComposite()) {
        const Folder* f = static_cast<Folder*>(fs);
        if (f != nullptr)
          f->getChildrensRecursive(out);
      } else {
        out.push_back(fs);
      }
    }
  }

 public:
  explicit Folder(const std::string& name) : FileSystem{name} {}

  ~Folder() {
    // Delete folder should delete all children
    for (auto f : _children) {
      std::cout << "Folder '" << this->getName()
                << "' deleted : " << f->getName() << "\n";
      delete f;
    }
  }

  void add(FileSystem* fs) override {
    std::cout << "Folder '" << this->getName() << "' added : " << fs->getName()
              << "\n";
    _children.push_back(fs);
    fs->setParent(this);
  }

  void remove(FileSystem* fs) override {
    std::cout << "Folder: " << this->getName() << "removed : " << fs->getName()
              << "\n";
    _children.remove(fs);
    fs->setParent(nullptr);
  }

  void open() const override {
    std::cout << "Open Folder: " << this->getName() << "\n";
  }

  int size() const override {
    int size = static_cast<int>(_children.size());
    std::for_each(_children.begin(), _children.end(), [&size](FileSystem* fs) {
      if (fs->isComposite()) {
        const Folder* f = static_cast<Folder*>(fs);
        if (f != nullptr)
          size += f->size();
      }
    });

    return size;
  }
};

/**
 * The Leaf class represents the end objects of a composition. A leaf can't have
 * any children.
 *
 * Usually, it's the Leaf objects that do the actual work, whereas Composite
 * objects only delegate to their sub-components.
 */
class File : public FileSystem {
 public:
  explicit File(const std::string& name) : FileSystem{name} {}

  void open() const override {
    std::cout << "Open File: " << this->getName() << "\n";
  }
};

class ZipFile : public FileSystem {
 public:
  explicit ZipFile(const std::string& name) : FileSystem{name} {}

  void open() const override {
    std::cout << "Open ZipFile: " << this->getName() << "\n";
  }
};

class ShortCut : public FileSystem {
 private:
  FileSystem* origin;

 public:
  explicit ShortCut(FileSystem* fs) : FileSystem{fs->getName()} { origin = fs; }

  void open() const override {
    std::cout << "Open ShortCut: " << this->getName() << "\n";
    if (origin != nullptr) {
      std::cout << "Navigate to: " << origin->getName() << "\n";
    } else {
      std::cout << "Original file no longer exist\n";
    }
  }
};

namespace Client {
void clientCode(const FileSystem* fs) {
  std::cout << "File name: " << fs->getName() << ", size: " << fs->size()
            << "\n";
  fs->open();
}
}  // namespace Client

void run() {
  std::cout << "\n\n";
  FileSystem* root = new Folder("root");
  root->open();

  // Prepare files
  FileSystem* file1 = new File("file_name.txt");
  FileSystem* file2 = new ZipFile("zipfile_name.txt");
  FileSystem* file3 = new ShortCut(file2);
  root->add(file1);
  root->add(file2);
  root->add(file3);
  Client::clientCode(file1);
  Client::clientCode(root);

  Folder* subFolder1 = new Folder("subFolder1");
  Folder* subFolder2 = new Folder("subFolder2");
  Folder* subFolder3 = new Folder("subFolder3");
  root->add(subFolder1);
  root->add(subFolder2);
  root->add(subFolder3);
  Client::clientCode(root);

  root->remove(file1);
  delete file1;

  root->remove(file2);
  delete file2;

  root->remove(file3);
  delete file3;

  delete root;  // deletes all files/subfolders inside recursively
}

}  // namespace CompositePattern

}  // namespace

struct CompositeAutoRuner {
  CompositeAutoRuner() {
    std::cout << "\n--- Composite Pattern Example ---\n";
    Problem::run();
    CompositePattern::run();
  }
};

static CompositeAutoRuner instance;