#ifndef INTEGRATION_GUI_COLLAPSIBLE_ELEMENT_H
#define INTEGRATION_GUI_COLLAPSIBLE_ELEMENT_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

/**
 * A Widget with a title and a description field. The custom content can be hidden but
 * the title and the description will stay.
 */
class CollapsibleElement : public QWidget {
 Q_OBJECT
 private:
  /**
   * Label vor the Widget title.
   */
  QLabel *lbl_title_ = nullptr;

  /**
   * Label for the widget description.
   */
  QLabel *lbl_description_ = nullptr;

  /**
   * Button to toggle the content visible or hide it.
   */
  QPushButton *btn_toggle_ = nullptr;

  /**
   * Layout of the body to display custom content.
   */
  QVBoxLayout *layout_body_ = nullptr;

  /**
   * Widget for the custom content.
   */
  QWidget *widget_body_ = nullptr;

  /**
   * Indicator for status of widget (hidden|visible).
   */
  bool collapsed = false;

  /**
   * Will perform the hiding of the custom content.
   */
  void collapse();

  /**
   * Will perform the redisplay of the custom content.
   */
  void expand();

  /**
   * Setting up Gui
   */
  void setUpGui();

  /**
   * Connect all inner functions
   */
  void setUpConnection();
 public:
  /**
   * Setup widget gui and connections.
   * @param parent parent widget for deallocation of pointers
   */
  explicit CollapsibleElement(QWidget *parent = nullptr);

  /**
   * Set text of title label.
   * @param title new text
   */
  void setTitle(const QString& title);

  /**
   * Set Text of description label.
   * @param description new description
   */
  void setDescription(const QString& description);

  /**
   * The Layout of the container for the custom content.
   * @return HVBoxLayout of custom content widget
   */
  QVBoxLayout &getLayout();

 private slots:

  /**
   * Slot to perform the toggling from visible to hidden.
   */
  void toggleStatus();
};

#endif //INTEGRATION_GUI_COLLAPSIBLE_ELEMENT_H
