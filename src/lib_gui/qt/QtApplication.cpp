#include "QtApplication.h"
// Qt5
#include <QFileOpenEvent>
// internal
#include "FilePath.h"
#include "LogManager.h"
#include "MessageLoadProject.h"
#include "MessageWindowFocus.h"
#include "utilityApp.h"

QtApplication::QtApplication(int& argc, char** argv) : QApplication(argc, argv) {
  connect(this, &QGuiApplication::applicationStateChanged, [](auto state) {
    MessageWindowFocus(state == Qt::ApplicationActive).dispatch();
  });
  Q_INIT_RESOURCE(resources);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
int QtApplication::exec() {
  return QApplication::exec();
}

bool QtApplication::event(QEvent* pEvent) {
  if(pEvent->type() == QEvent::FileOpen) {
    auto* pFileEvent = dynamic_cast<QFileOpenEvent*>(pEvent);

    FilePath path(pFileEvent->file().toStdWString());

    if(path.exists() && path.extension() == L".srctrlprj") {
      MessageLoadProject(path).dispatch();
      return true;
    }
  }

  return QApplication::event(pEvent);
}
