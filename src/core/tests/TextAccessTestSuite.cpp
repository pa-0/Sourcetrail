#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TextAccess.h"

namespace {
std::string getTestText() {
  std::string text =
      "\"But the plans were on display . . .\"\n"
      "\"On display? I eventually had to go down to the cellar to find them.\"\n"
      "\"That's the display department.\"\n"
      "\"With a torch.\"\n"
      "\"Ah, well the lights had probably gone.\"\n"
      "\"So had the stairs.\"\n"
      "\"But look, you found the notice, didn't you?\"\n"
      "\"Yes,\" said Arthur, \"yes I did. It was on display in the bottom of a locked"
      " filing cabinet stuck in a disused lavatory with a sign on the door saying"
      " Beware of the Leopard.\"\n";

  return text;
}
}    // namespace

// NOLINTNEXTLINE(cert-err58-cpp)
TEST(TextAccess, textAccessStringConstructor) {
  std::string text = getTestText();

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromString(text);

  EXPECT_TRUE(textAccess.get() != nullptr);
}

TEST(TextAccess, textAccessStringLinesCount) {
  std::string text = getTestText();
  const unsigned int lineCount = 8;

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromString(text);

  EXPECT_TRUE(textAccess->getLineCount() == lineCount);
}

TEST(TextAccess, textAccessStringLinesContent) {
  std::string text = getTestText();

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromString(text);
  std::vector<std::string> lines = textAccess->getLines(1, 4);

  EXPECT_TRUE(lines.size() == 4);
  EXPECT_TRUE(lines[0] == "\"But the plans were on display . . .\"\n");
  EXPECT_TRUE(lines[1] == "\"On display? I eventually had to go down to the cellar to find them.\"\n");
  EXPECT_TRUE(lines[2] == "\"That's the display department.\"\n");
  EXPECT_TRUE(lines[3] == "\"With a torch.\"\n");
}

TEST(TextAccess, textAccessStringLinesContentErrorHandling) {
  std::string text = getTestText();

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromString(text);
  std::vector<std::string> lines = textAccess->getLines(3, 2);

  EXPECT_TRUE(lines.empty());

  lines = textAccess->getLines(10, 3);

  EXPECT_TRUE(lines.empty());

  lines = textAccess->getLines(1, 10);

  EXPECT_TRUE(lines.empty());

  std::string line = textAccess->getLine(0);

  EXPECT_TRUE(line.empty());

  lines = textAccess->getLines(0, 2);

  EXPECT_TRUE(line.empty());
}

TEST(TextAccess, TextAccessStringSingleLineContent) {
  std::string text = getTestText();

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromString(text);
  const std::string line = textAccess->getLine(6);

  EXPECT_TRUE(line == "\"So had the stairs.\"\n");
}

TEST(TextAccess, textAccessStringAllLines) {
  std::string text = getTestText();
  const unsigned int lineCount = 8;

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromString(text);
  std::vector<std::string> lines = textAccess->getAllLines();

  EXPECT_TRUE(lines.size() == lineCount);
}

TEST(TextAccess, textAccessFileConstructor) {
  FilePath filePath(L"data/TextAccessTestSuite/text.txt");

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromFile(filePath);

  EXPECT_TRUE(textAccess.get() != nullptr);
}

TEST(TextAccess, textAccessFileLinesCount) {
  FilePath filePath(L"data/TextAccessTestSuite/text.txt");
  const unsigned int lineCount = 7;

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromFile(filePath);

  EXPECT_TRUE(textAccess->getLineCount() == lineCount);
}

TEST(TextAccess, textAccessFileLinesContent) {
  FilePath filePath(L"data/TextAccessTestSuite/text.txt");

  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromFile(filePath);
  std::vector<std::string> lines = textAccess->getLines(1, 4);

  EXPECT_TRUE(lines.size() == 4);
  EXPECT_TRUE(lines[0] ==
              "\"If you're a researcher on this book thing and you were on Earth, you must have been "
              "gathering material on it.\"\n");
  EXPECT_TRUE(lines[1] == "\"Well, I was able to extend the original entry a bit, yes.\"\n");
  EXPECT_TRUE(lines[2] == "\"Let me see what it says in this edition, then. I've got to see it.\"\n");
  EXPECT_TRUE(lines[3] ==
              "... \"What? Harmless! Is that all it's got to say? Harmless! One word! ... Well, for "
              "God's sake I hope you managed to rectify that a bit.\"\n");
}

TEST(TextAccess, textAccessFileGetFilePath) {
  FilePath filePath(L"data/TextAccessTestSuite/text.txt");
  std::shared_ptr<TextAccess> textAccess = TextAccess::createFromFile(filePath);

  EXPECT_TRUE(textAccess->getFilePath() == filePath);
}