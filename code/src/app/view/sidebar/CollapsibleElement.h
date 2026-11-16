#ifndef INTEGRATION_GUI_COLLAPSIBLE_ELEMENT_H
#define INTEGRATION_GUI_COLLAPSIBLE_ELEMENT_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QToolButton>

/**
 * A Widget with a title and a description field. The custom content can be hidden but
 * the title and the description will stay.
 */
class CollapsibleElement : public QWidget {
 Q_OBJECT
 private:
  /**
   * Title button with a disclosure arrow, toggles the content.
   */
  QToolButton *btn_title_ = nullptr;

  /**
   * Label for the widget description.
   */
  QLabel *lbl_description_ = nullptr;

  /**
   * Layout of the body to display custom content.
   */
  QFormLayout *layout_body_ = nullptr;

  /**
   * Widget for the custom content.
   */
  QWidget *widget_body_ = nullptr;

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
   * @return QFormLayout of custom content widget
   */
  QFormLayout &getLayout();

 private slots:

  /**
   * Slot to show or hide the custom content.
   * @param expanded true to show the content
   */
  void setExpanded(bool expanded);
};

#endif //INTEGRATION_GUI_COLLAPSIBLE_ELEMENT_H
