#include "ASTConsumer.h"
// internal
#include "ApplicationSettings.h"
#include "CxxAstVisitor.h"
#include "CxxVerboseAstVisitor.h"

ASTConsumer::ASTConsumer(clang::ASTContext* context,
                         clang::Preprocessor* preprocessor,
                         std::shared_ptr<ParserClient> client,
                         std::shared_ptr<CanonicalFilePathCache> canonicalFilePathCache,
                         std::shared_ptr<IndexerStateInfo> indexerStateInfo) {
  auto* pAppSettings = ApplicationSettings::getInstance().get();

  if(pAppSettings->getLoggingEnabled() && pAppSettings->getVerboseIndexerLoggingEnabled()) {
    m_visitor = std::make_shared<CxxVerboseAstVisitor>(
        context, preprocessor, client, canonicalFilePathCache, indexerStateInfo);
  } else {
    m_visitor = std::make_shared<CxxAstVisitor>(
        context, preprocessor, client, canonicalFilePathCache, indexerStateInfo);
  }
}

ASTConsumer::~ASTConsumer() = default;

void ASTConsumer::HandleTranslationUnit(clang::ASTContext& context) {
  m_visitor->indexDecl(context.getTranslationUnitDecl());
}