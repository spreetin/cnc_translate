#ifndef COLORCODER_H
#define COLORCODER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QVector>
#include <QRegularExpression>

class ISOHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    ISOHighlighter(QTextDocument * parent = nullptr);

    void setMultiletter(std::set<std::string_view> allowed_ml);

protected:
    void highlightBlock(const QString &text) override;

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    static const QVector<HighlightingRule> rules;
    std::set<QString> ml;
    QTextCharFormat ml_format;
};

#endif //COLORCODER_H
