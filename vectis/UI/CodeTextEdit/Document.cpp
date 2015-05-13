#include <UI/CodeTextEdit/Document.h>
#include <UI/CodeTextEdit/CodeTextEdit.h>
#include <QFile>
#include <QTextStream>

#include <QDebug>

Document::Document(const CodeTextEdit& codeTextEdit) :
  m_codeTextEdit(codeTextEdit),
  m_wrapWidth(-1)
{
  //qDebug() << m_codeTextEdit.fontMetrics().maxWidth() << " " << m_codeTextEdit.getViewportWidth();
}

// The following function loads the contents of a text file into memory.
// This is a memory-expensive operation but documents need to be available at any time
// Returns true on success
bool Document::loadFromFile(QString file) {

  QFile f(file);
  if (f.open(QFile::ReadWrite | QFile::Text) == false)
    return false;

  QTextStream in(&f);
  // Load the entire file into memory (expensive but necessary)
  m_plainText = in.readAll();

  return true;
}

// Triggers a lines/breakpoints recalculation for the entire document. -1 means
// "no wrap" and it's the default
void Document::setWrapWidth(int width) {
  m_wrapWidth = width;
  qDebug() << "setWrapWidth() to " << width;
  // TODO trigger recalculate
}


void Document::recalculateDocument () {
  // Scan each line (until a newline is found)
  QTextStream ss(&m_plainText);
  QString line;
  do {
    line = ss.readLine();
    if (line.isNull())
      continue;

    if (m_wrapWidth != -1 && line.count() > m_wrapWidth) {
      // We have a wrap and the line is too big - WRAP IT

      // TODO

    } else {
      // No wrap or the line fits perfectly within the wrap limits

      m_physicalLines.emplace_back( PhysicalLine ( EditorLine(line) ) );
    }

  } while(line.isNull() == false);

  // At this point the PhysicalLines vector has been populated (or is still empty)
  // TODO something else
}


/*
 *
 * Document represents a grid for a text file
 *
 * It provides a vector of PhysicalLine objects (the physical ones) that can include one or more multiple
 * EditorLine objects (the fake ones due to wrap). A PhysicalLine has two values: an int beginValue (i.e. the
 * starting pixel line where the line begins to be rendered in a virtual document) and an int endValue (after
 * all the EditorLines have been rendered). fontMetrics() and m_wrapWidth are the values necessary to calculate
 * this grid.
 *
 * document_is_loaded()
 *  1) From its extension (e.g. cpp) a Lexer is chosen (or no lexer at all). If chosen it is lexed.
 *
 *  2) PhysicalLine objects are created, each one with ONE or MORE EditorLine. An EditorLine contains a list of words
 *     and their associated style (bold/italic/color/etc..) due to how the Lexer interpreted this.
 *
 *  3) PhysicalLine objects also have a beginValue and endValue (to facilitate queries when something is clicked inside
 *     the rendered region). After a click was received, a search for the character (or no-character in the line) is issued
 *     so the Document must have something like
 *       clickOnPoint(Point, hint LinesFrom, hint LinesTo (these hints are given since the viewport displays only a few lines) ->
 *         return the new click position
 *       doubleClickOnPoint (..) -> return the start-end positions for a selection (or a list of words in the entire document for
 *                                  similar selections)
 *       startDragSelection ().. etc.
 *
 *
 */


// Copy the characters for this editor line into our characters vector
EditorLine::EditorLine(QString str) {
  m_characters.resize(str.length());
  std::copy(str.begin(), str.end(), m_characters.begin());
}
