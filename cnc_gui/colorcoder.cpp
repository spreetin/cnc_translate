#include "colorcoder.h"

const QVector<ISOHighlighter::HighlightingRule> ISOHighlighter::rules = {
    {QRegularExpression("[ABCDEFHIJKLOPQRSTUVWXYZ]", QRegularExpression::CaseInsensitiveOption), []{
         QTextCharFormat fmt;
         fmt.setForeground(Qt::blue);
         return fmt;
     }()},
    {QRegularExpression("G\\d{1,4}", QRegularExpression::CaseInsensitiveOption), []{
         QTextCharFormat fmt;
         fmt.setForeground(Qt::darkMagenta);
         return fmt;
     }()},
    {QRegularExpression("M\\d{1,4}", QRegularExpression::CaseInsensitiveOption), []{
         QTextCharFormat fmt;
         fmt.setForeground(Qt::darkGreen);
         return fmt;
     }()},
    {QRegularExpression("[LV]\\d{1,4}", QRegularExpression::CaseInsensitiveOption), []{
         QTextCharFormat fmt;
         fmt.setForeground(Qt::gray);
         return fmt;
     }()},
    {QRegularExpression(R"(^[Nn]\d{1,4}(?:\!.*|\s|$))", QRegularExpression::CaseInsensitiveOption), []{
         QTextCharFormat fmt;
         fmt.setForeground(Qt::darkYellow);
         return fmt;
     }()},
    {QRegularExpression("\\(.*$", QRegularExpression::CaseInsensitiveOption), []{
         QTextCharFormat fmt;
         fmt.setForeground(Qt::gray);
         return fmt;
     }()},
};

ISOHighlighter::ISOHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    ml_format.setForeground(Qt::blue);
}

void ISOHighlighter::setMultiletter(std::set<std::string_view> allowed_ml)
{
    for (const auto &sv : allowed_ml){
        ml.insert(QString::fromUtf8(sv.data(), sv.size()));
    }
}

void ISOHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : rules){
        auto match = rule.pattern.match(text);
        while (match.hasMatch()){
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            match = rule.pattern.match(text, match.capturedEnd());
        }
    }
    for (const auto &func : ml){
        int pos = -1;
        while ((pos = text.indexOf(func, pos+1, Qt::CaseInsensitive)) != -1){
            setFormat(pos, func.length(), ml_format);
        }
    }
}
