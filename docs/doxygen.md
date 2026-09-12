# API documentation with Doxygen

The framework headers in `include/lab/` are documented with `///` comments, and every example is
browsable as source. Doxygen turns both into a searchable HTML site: the front page is the project
[README](../README.md), the guides become "Related Pages", and "Files" lets you read any example
with cross-linked identifiers.

---
## 1. Generate it locally

```bash
sudo apt-get install doxygen graphviz        # graphviz (dot) is optional: it draws the class graphs
cmake -S . -B build
cmake --build build --target docs
xdg-open build/docs/html/index.html
```

The target is created only when Doxygen is found, and it is not part of `all`, so a normal build
never pays for it. Everything lands in `build/docs/` (git-ignored):

| Path                                | Content                                  |
| ----------------------------------- | ---------------------------------------- |
| `build/Doxyfile`                     | generated from `docs/Doxyfile.in`        |
| `build/docs/html/index.html`         | the site                                 |
| `build/docs/doxygen-warnings.log`    | warnings (empty when everything is fine) |

CMake options:

| Option                            | Default | Meaning                                        |
| --------------------------------- | ------- | ---------------------------------------------- |
| `CPPLAB_BUILD_DOCS`               | `ON`    | add the `docs` target when Doxygen is installed |
| `CPPLAB_DOCS_WARNINGS_AS_ERRORS`  | `OFF`   | fail the target on Doxygen warnings (CI uses `ON`) |

To run Doxygen without CMake, use the generated file: `doxygen build/Doxyfile`. Do not edit it,
it is overwritten on every configure; change `docs/Doxyfile.in` instead.

---
## 2. How the sources are documented

Doxygen reads `///`, `//!` and `/** ... */` comments. The lab uses them like this:

```cpp
/// Collection of all examples.
///
/// Examples register themselves through LAB_EXAMPLE while the program starts.
class Registry {
 public:
  /// Registers one example.
  /// @param file        __FILE__ of the calling source file.
  /// @param name        short name shown in the menu.
  /// @return false when the example was rejected (see errors()).
  bool add(std::string_view file, std::string_view name, ...);
};
```

Useful commands inside a comment: `@brief`, `@param`, `@return`, `@note`, `@warning`, `@see`,
`@code ... @endcode`, `@ref other-page`.

The ~100 example files deliberately keep their teaching text in plain `//` comments: they explain a
concept in order, not an API. `EXTRACT_ALL` and `SOURCE_BROWSER` are on, so those files still appear
under "Files" with their comments intact - no rewriting needed. When you add a header to
`include/lab/`, document it with `///`, because that is what becomes the public API reference.

---
## 3. CI: publish to GitHub Pages

`.github/workflows/docs.yml` builds the site on every push and pull request (warnings are errors
there) and, on pushes to `master`, deploys it to GitHub Pages.

**One-time setup in the repository (a maintainer has to do this once):**

1. Open **Settings -> Pages**.
2. Under **Build and deployment -> Source**, choose **GitHub Actions**.
3. Push to `master` (or run the workflow manually from the **Actions** tab).

The site is then served at `https://<owner>.github.io/<repo>/` - for this repository,
<https://urbytes21.github.io/cpp-lab/>. The workflow needs no token: it uses the `pages: write`
and `id-token: write` permissions declared in the job.

If the deploy step fails with "Pages is not enabled", step 2 above has not been done yet. The build
step still succeeds and uploads the site as a workflow artifact, so you can download it from the
run page.

---
## 4. Link it from the GitHub wiki

The wiki only stores Markdown - it cannot host the generated HTML - so the usual approach is to
keep the wiki as the hand-written entry point and link it to the generated site.

**Manually:**

1. Open the **Wiki** tab and create the **Home** page (this also creates the wiki repository).
2. Paste something like:

   ```markdown
   # cpp-lab

   - [API documentation](https://urbytes21.github.io/cpp-lab/) (generated with Doxygen on every push to master)
   - [Learning path](https://github.com/urbytes21/cpp-lab/blob/master/docs/learning-path.md)
   - [Adding examples](https://github.com/urbytes21/cpp-lab/blob/master/docs/adding-examples.md)
   ```

**Automatically**, with the helper script:

```bash
./scripts/publish_wiki.sh                      # uses the origin remote
./scripts/publish_wiki.sh https://github.com/<owner>/<repo>.wiki.git
```

It clones `<repo>.wiki.git`, regenerates `Home.md` from the repository documentation (the docs
index plus a link to the Pages site) and pushes if anything changed. The wiki is a normal git
repository, so a push needs credentials with write access to the repository.

> To run it from CI as well, add a step that calls the script with a personal access token in the
> URL (`https://x-access-token:${{ secrets.WIKI_TOKEN }}@github.com/<owner>/<repo>.wiki.git`). The
> default `GITHUB_TOKEN` cannot write to the wiki.

---
## 5. Troubleshooting

| Symptom                                  | Cause and fix                                                      |
| ---------------------------------------- | ------------------------------------------------------------------ |
| No `docs` target                          | Doxygen is not installed, or `-DCPPLAB_BUILD_DOCS=OFF`             |
| No class diagrams                         | graphviz is missing; install it and re-run `cmake -S . -B build`    |
| `warning: ... does not exist`             | a path in `DOXYGEN_INPUT` (`cmake/Docs.cmake`) points at a deleted file |
| CI docs job fails but the site looks fine | a real Doxygen warning: read `build/docs/doxygen-warnings.log`      |
