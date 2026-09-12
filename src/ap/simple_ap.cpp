#include <gtkmm.h>
#include <iostream>
#include <string>

// We put everything into a single window class (God Class)
class MainWindow : public Gtk::Window {

  // --- Layout Containers ---
  Gtk::Box m_mainLayout_;
  Gtk::Box m_topRowLayout_;

  // --- Widgets ---
  // We must declare everything here so that the buttons are accessible

  // Editor
  Gtk::Frame frame_Editor_;
  Gtk::Box m_boxEditor_;
  Gtk::Entry entry_Input_;
  Gtk::Button button_Update_;

  // Monitor A
  Gtk::Frame frame_MonitorA_;
  Gtk::Box m_boxMonitorA_;
  Gtk::Label m_labelMonitorA_;

  //  Monitor B
  Gtk::Frame frame_MonitorB_;
  Gtk::Box m_boxMonitorB_;
  Gtk::Label m_labelMonitorB_;

 public:
  MainWindow()
      : m_mainLayout_(Gtk::Orientation::VERTICAL),
        m_topRowLayout_(Gtk::Orientation::HORIZONTAL),
        m_boxEditor_(Gtk::Orientation::VERTICAL),
        m_boxMonitorA_(Gtk::Orientation::VERTICAL),
        m_boxMonitorB_(Gtk::Orientation::VERTICAL) {
    set_title("No-MVC (Coupled) Demo");
    set_default_size(600, 400);

    // 1. SETUP UI (Visually identical to MVC)
    // --- Monitor A ---
    frame_MonitorA_.set_label("ZONE 2: MONITOR A (Blue)");
    m_labelMonitorA_.set_markup(
        "<span foreground='blue' size='x-large'>Initial Data</span>");
    m_boxMonitorA_.append(m_labelMonitorA_);
    frame_MonitorA_.set_child(m_boxMonitorA_);
    frame_MonitorA_.set_hexpand(true);  // Stretch

    // --- Monitor B ---
    frame_MonitorB_.set_label("ZONE 3: MONITOR B (Red)");
    m_labelMonitorB_.set_markup(
        "<span foreground='red' size='x-large'>Initial Data</span>");
    m_boxMonitorB_.append(m_labelMonitorB_);
    frame_MonitorB_.set_child(m_boxMonitorB_);
    frame_MonitorB_.set_hexpand(true);

    // --- Editor ---
    frame_Editor_.set_label("ZONE 1: EDITOR");
    entry_Input_.set_text("Initial Data");
    button_Update_.set_label("Direct Update");  // Live updates
    m_boxEditor_.append(entry_Input_);
    m_boxEditor_.append(button_Update_);
    frame_Editor_.set_child(m_boxEditor_);

    // --- Layout ---
    m_topRowLayout_.append(frame_MonitorA_);
    m_topRowLayout_.append(frame_MonitorB_);
    m_mainLayout_.append(m_topRowLayout_);
    m_mainLayout_.append(frame_Editor_);

    // Margin for aesthetics
    m_boxEditor_.set_margin(10);
    m_boxEditor_.set_spacing(5);
    m_boxMonitorA_.set_margin(20);
    m_boxMonitorB_.set_margin(20);
    set_child(m_mainLayout_);

    // 2. LOGIC HANDLING
    // Here, the button must "know" exactly who m_labelMonitorA and m_labelMonitorB are.
    // It directly controls the other widgets.
    // (THE BAD PART)
    button_Update_.signal_clicked().connect([this]() {
      // Step 1: Get data directly from UI (Entry)
      std::string text = entry_Input_.get_text();
      // There may be processing logic here (Validating...)
      if (text.empty())
        return;

      // Step 2: Update Monitor A (Hard-coded) directly
      const std::string escaped = Glib::Markup::escape_text(text).raw();
      m_labelMonitorA_.set_markup("<span foreground='blue' size='x-large'>" +
                                  escaped + "</span>");

      // Step 3: Update Monitor B (Hard-coded) directly
      m_labelMonitorB_.set_markup("<span foreground='red' size='x-large'>" +
                                  escaped + "</span>");
      std::cout << "Updated directly without Model!" << std::endl;
    });
  }
};

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.example.nomvc");
  return app->make_window_and_run<MainWindow>(argc, argv);
}