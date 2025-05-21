#include <QRegularExpression>
#include <QToolTip>
#include <map>
#include "cnctextdocument.h"
#include "../tokens/g_code_description.h"
#include "../tokens/m_code_descriptions.h"

CNCTextDocument::CNCTextDocument(QWidget *parent) : QPlainTextEdit(parent) {
    setMouseTracking(true);
}

bool CNCTextDocument::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip){
        auto * event = static_cast<QHelpEvent*>(e);
        QPoint pos = event->pos();
        pos.setX(pos.x() - viewportMargins().left());
        pos.setY(pos.y() - viewportMargins().top());
        QTextCursor cursor = cursorForPosition(pos);
        cursor.select(QTextCursor::WordUnderCursor);
        QString text = cursor.selectedText();
        static QRegularExpression gRE = QRegularExpression("[Gg]\\d{1,3}");
        static QRegularExpression mRE = QRegularExpression("[Mm]\\d{1,3}");
        if (text.startsWith('G', Qt::CaseInsensitive)){
            auto match = gRE.match(text);
            if (match.hasMatch()){
                text = text.mid(match.capturedStart()+1, match.capturedLength());
                bool ok;
                int code = text.toInt(&ok);
                if (ok){
                    for (const auto &g : param.g){
                        if (g.second.word.index() == 0 && std::get<0>(g.second.word) == code){
                            code = g.first;
                            auto view = NCParser::g_code_name((NCParser::GCodesAvailable)code);
                            text = QString::fromUtf8(view.data(), view.size());
                            if (!text.isEmpty()){
                                QToolTip::showText(event->globalPos(), text);
                                return true;
                            }
                        }
                    }
                }
            }

        } else if (text.startsWith('M', Qt::CaseInsensitive)){
            auto match = mRE.match(text);
            if (match.hasMatch()){
                text = text.mid(match.capturedStart()+1, match.capturedLength());
                bool ok;
                int code = text.toInt(&ok);
                if (ok){
                    for (const auto &m : param.m){
                        if (m.second.num == code){
                            auto view = NCParser::m_code_name((NCParser::MCodesAvailable)m.first);
                            text = QString::fromUtf8(view.data(), view.size());
                            if (!text.isEmpty()){
                                QToolTip::showText(event->globalPos(), text);
                            } else {
                                QToolTip::hideText();
                            }
                            return true;
                        }
                    }
                }
                if (ok && param.m.contains(code)){
                    code = param.m[code].num;
                    auto view = NCParser::m_code_name((NCParser::MCodesAvailable)code);
                    text = QString::fromUtf8(view.data(), view.size());
                    if (!text.isEmpty()){
                        QToolTip::showText(event->globalPos(), text);
                    } else {
                        QToolTip::hideText();
                    }
                    return true;
                }
            }
        } else {
            QToolTip::hideText();
        }
    }
    return QPlainTextEdit::event(e);
}
