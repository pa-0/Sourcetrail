```plantuml
@startuml

!include https://raw.githubusercontent.com/patrik-csak/one-dark-plantuml-theme/v1.0.1/theme.puml

package SQL <<Database>> {
}

package lib {
  class Controller
	class ConsoleView
	class ConsoleController
  class View
	class ConsoleParser
}

package lib_gui {
  class QtConsoleView
}

package Qt5 {
	class QWidget
}

Controller <|-- ConsoleController

View        <|-- ConsoleView
ConsoleView <|-- QtConsoleView
QWidget     <|-- QtConsoleView

ConsoleView o- ConsoleController

ConsoleController *- ConsoleParser
ConsoleParser <- SQL

@enduml
```

```plantuml
@startuml

!include https://raw.githubusercontent.com/patrik-csak/one-dark-plantuml-theme/v1.0.1/theme.puml

actor User as user

box "Application"
	participant Application
	participant MainView
	participant ComponentManager
	participant ComponentFactory
	participant QtViewFactory
	participant View
	participant QtConsoleView
end box

user             -> Application      : createInstance()
Application      -> MainView         : setup()
MainView         -> ComponentManager : setupMain()
ComponentManager -> ComponentFactory : createConsoleComponent()
ComponentFactory -> QtViewFactory
QtViewFactory    -> QtViewFactory
QtViewFactory    -> View
View             -> QtConsoleView

@enduml
```