#pragma once

#include "QtProjectWizardContentPath.h"
#include "SingleValueCache.h"

class SourceGroupSettingsCxxCodeblocks;

class QtProjectWizardContentPathCodeblocksProject : public QtProjectWizardContentPath {
  Q_OBJECT

public:
  QtProjectWizardContentPathCodeblocksProject(std::shared_ptr<SourceGroupSettingsCxxCodeblocks> settings,
                                              QtProjectWizardWindow* window);

  // QtProjectWizardContent implementation
  void populate(QGridLayout* layout, int& row) override;

  void load() override;
  void save() override;

  std::vector<FilePath> getFilePaths() const override;
  QString getFileNamesTitle() const override;
  QString getFileNamesDescription() const override;

private slots:
  void pickedPath();

private:
  std::shared_ptr<SourceGroupSettings> getSourceGroupSettings() override;

  std::shared_ptr<SourceGroupSettingsCxxCodeblocks> m_settings;
  QLabel* m_fileCountLabel;
  mutable SingleValueCache<std::vector<FilePath>> m_filePaths;
};
