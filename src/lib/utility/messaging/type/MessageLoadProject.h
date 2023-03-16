#pragma once

#include "RefreshInfo.h"

#include "FilePath.h"
#include "Message.h"

class MessageLoadProject : public Message<MessageLoadProject> {
public:
  MessageLoadProject(const FilePath& filePath,
                     bool settingsChanged_ = false,
                     RefreshMode refreshMode_ = REFRESH_NONE,
                     bool shallowIndexingRequested_ = false)
      : projectSettingsFilePath(filePath)
      , settingsChanged(settingsChanged_)
      , refreshMode(refreshMode_)
      , shallowIndexingRequested(shallowIndexingRequested_) {}

  static const std::string getStaticType() {
    return "MessageLoadProject";
  }

  void print(std::wostream& os) const override {
    os << projectSettingsFilePath.wstr();
    os << L", settingsChanged: " << std::boolalpha << settingsChanged;
    os << L", refreshMode: " << refreshMode;
  }

  const FilePath projectSettingsFilePath;
  const bool settingsChanged;
  const RefreshMode refreshMode;
  const bool shallowIndexingRequested;
};
