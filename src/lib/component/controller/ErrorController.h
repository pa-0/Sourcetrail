#pragma once
// STL
#include <map>
// internal
#include "ActivationListener.h"
#include "MessageErrorCountClear.h"
#include "MessageErrorCountUpdate.h"
#include "MessageErrorsAll.h"
#include "MessageErrorsForFile.h"
#include "MessageErrorsHelpMessage.h"
#include "MessageIndexingFinished.h"
#include "MessageIndexingStarted.h"
#include "MessageListener.h"
#include "MessageShowError.h"
#include "QtThreadedFunctor.h"
//
#include "Controller.h"
#include "ErrorView.h"

class StorageAccess;

class ErrorController final
    : public Controller
    , public ActivationListener
    , public MessageListener<MessageErrorCountClear>
    , public MessageListener<MessageErrorCountUpdate>
    , public MessageListener<MessageErrorsAll>
    , public MessageListener<MessageErrorsForFile>
    , public MessageListener<MessageErrorsHelpMessage>
    , public MessageListener<MessageIndexingFinished>
    , public MessageListener<MessageIndexingStarted>
    , public MessageListener<MessageShowError> {
public:
  explicit ErrorController(StorageAccess* pStorageAccess);

  ~ErrorController() override;

  void errorFilterChanged(const ErrorFilter& filter);

  void showError(Id errorId);

private:
  /** @name Handle Messages
   * @{
   */
  void handleActivation(const MessageActivateBase* pMessage) override;

  void handleMessage(MessageActivateErrors* pMessage) override;
  void handleMessage(MessageErrorCountClear* pMessage) override;
  void handleMessage(MessageErrorCountUpdate* pMessage) override;
  void handleMessage(MessageErrorsAll* pMessage) override;
  void handleMessage(MessageErrorsForFile* pMessage) override;
  void handleMessage(MessageErrorsHelpMessage* pMessage) override;
  void handleMessage(MessageIndexingFinished* pMessage) override;
  void handleMessage(MessageIndexingStarted* pMessage) override;
  void handleMessage(MessageShowError* pMessage) override;
  /**
   * @}
   */

  ErrorView* getView() const;

  void clear() override;

  bool showErrors(const ErrorFilter& filter, bool scrollTo);

  StorageAccess* m_storageAccess;

  size_t m_errorCount = 0;

  std::map<Id, bool> m_tabShowsErrors;
  std::map<Id, FilePath> m_tabActiveFilePath;

  QtThreadedLambdaFunctor m_onQtThread;
  bool m_newErrorsAdded = false;
};