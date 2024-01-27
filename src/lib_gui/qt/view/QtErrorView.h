#pragma once
// Qt5
#include <QWidget>
// internal
#include "ControllerProxy.h"
#include "ErrorController.h"
#include "ErrorFilter.h"
#include "ErrorView.h"
#include "QtThreadedFunctor.h"

class QBoxLayout;
class QCheckBox;
class QLabel;
class QPushButton;
class QStandardItemModel;
class QtHelpButton;
class QtSelfRefreshIconButton;
class QtTable;

class QtErrorView final
    : public QWidget
    , public ErrorView {
  Q_OBJECT

public:
  explicit QtErrorView(ViewLayout* viewLayout);

  ~QtErrorView() override;

  // View implementation
  void createWidgetWrapper() override;
  void refreshView() override;

  // ErrorView implementation
  void clear() override;

  void addErrors(const std::vector<ErrorInfo>& errors, const ErrorCountInfo& errorCount, bool scrollTo) override;
  void setErrorId(Id errorId) override;

  ErrorFilter getErrorFilter() const override;
  void setErrorFilter(const ErrorFilter& filter) override;

private slots:
  void errorFilterChanged(int i = 0);

private:
  enum class Column : int {
    ID = 0,
    TYPE = 1,
    MESSAGE = 2,
    FILE = 3,
    LINE = 4,
    INDEXED = 5,
    TRANSLATION_UNIT = 6,
    COLUMN_MAX = TRANSLATION_UNIT
  };

  void setStyleSheet() const;

  void addErrorToTable(const ErrorInfo& error);

  QCheckBox* createFilterCheckbox(const QString& name, bool checked, QBoxLayout* layout);
  bool isShownError(const ErrorInfo& error);

  static QIcon s_errorIcon;

  ControllerProxy<ErrorController> m_controllerProxy;
  QtThreadedLambdaFunctor m_onQtThread;

  ErrorFilter m_errorFilter;

  QLabel* m_errorLabel;
  QLabel* m_allLabel;
  QPushButton* m_allButton;

  QCheckBox* m_showErrors;
  QCheckBox* m_showFatals;
  QCheckBox* m_showNonIndexedErrors;
  QCheckBox* m_showNonIndexedFatals;

  QtHelpButton* m_helpButton;
  QtSelfRefreshIconButton* m_editButton;

  QStandardItemModel* m_model;
  QtTable* m_table;
};