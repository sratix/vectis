#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <utility>

class CodeTextEdit;

class EditorLine {
public:
  EditorLine (QString str);
private:
  std::vector<QChar> m_characters;
};

class PhysicalLine {
public:
  template<typename T>
  PhysicalLine (T&& editorLine) {
    static_assert(std::is_same<typename std::remove_reference<T>::type, EditorLine>::value,
                  "Expected EditorLine argument");
    m_editorLines.emplace_back(std::forward<T>(editorLine));
  }

private:
  std::vector<EditorLine> m_editorLines;
};

// This class represents document loaded from the CodeTextEdit control.
// A text document is treated as a grid of rectangles (the monospaced characters)
// and a text 'physical' line might include one or more editor lines due to wrap
// factors.
class Document : public QObject {
    Q_OBJECT
public:
    explicit Document(const CodeTextEdit& codeTextEdit);

    bool loadFromFile (QString file);
    void setWrapWidth(int width);

private:
    void recalculateDocument();

    QString m_plainText;
    const CodeTextEdit& m_codeTextEdit;
    int m_wrapWidth;
    std::vector<PhysicalLine> m_physicalLines;
};

#endif // DOCUMENT_H
