#include <cctype>
#include <cmath>
#include <stdexcept>
#include <string>
#include <set>
#include <algorithm>
#include "lexer.h"
#include "../tokens/tokens.h"

#ifdef TESTING
#define RETURN(x) tokenizedReturn(x)
#else
#define RETURN(x) x
#endif


namespace NCParser {

lexer::lexer() {}

/*!
 * \brief Initialize the lexer with a string to read, and a list of multi letter values accepted
 * \param text The text to parse
 * \param allowed_multiletters List of all allowed multi letter values to parse as single tokens
 */
void lexer::init(std::string text, std::set<std::string_view> allowed_multiletters)
{
    this->text = text;
    multiletter = allowed_multiletters;
    m_pos = 0;
    m_line = 0;
}

/*!
 * \brief Returns the next token found in the text
 *
 * If the token type found is associated with a value, this value can be grabbed
 * by calling intValue(), doubleValue() or stringValue() as appropriate afterwards.
 *
 * Tokens representing errors and comment blocks needs to be handled according to
 * special methods, using finish_line() and finish_comment().
 *
 * \return Token type
 */
int lexer::next()
{
    while (true){
        if (text.length() <= m_pos){
            return RETURN(Token::done);
        }
        m_last = m_next;
        m_next = std::toupper(text.at(m_pos));
        m_pos++;
        std::string ml_text;
        if (!((ml_text = checkML(m_next)).empty())){
            vValue = ml_text;
            return RETURN(Token::multi_letter);
        } else {
            switch (m_next){
            case ' ':
            case '\t':
            case '\r':
                continue;
            case 'G':
                return RETURN(getCode(Token::g_word));
            case 'M':
                return RETURN(getCode(Token::m_word));
            case 'N':
            {
                if (!std::isdigit(peek()))
                    continue;
                int val = std::stoi(grabInt());
                vValue = val;
                return RETURN(Token::n_word);
            }
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
                vValue = grabInt(-1);
                return RETURN(Token::num_literal);
            }
            case '\n':
                m_line++;
                if (m_last == '\n')
                    return RETURN(Token::empty_line);
                return RETURN(Token::newline);
            default:
                return RETURN(m_next);
            }
        }
    }
    m_last_error = {error::Lexing, m_line, pos(),
                    "Undefined token encountered on \""
                        + text.substr(pos(), text.find('\n', m_pos))};
    return Token::error;
}

/*!
 * \brief Whenever parsing of a block fails, this function can grab remaning text in the block
 */
void lexer::finish_line()
{
    int start = m_pos;
    while (m_pos < text.size() && text.at(m_pos) != '\n'){
        m_pos++;
    }
    vValue = text.substr(start, m_pos-start);
}

/*!
 * \brief Grabs all text contained in a comment block as a single string value
 *
 * Comments can't be multiline, so everything up to the first newline is grabbed,
 * unless a character matching the end parameter is found first.
 *
  * \param end Character marking end of the comment block
 */
void lexer::finish_comment(char end)
{
    int start = m_pos;
    while (m_pos < text.size() && text.at(m_pos) != '\n' && text.at(m_pos) != end){
        m_pos++;
    }
    vValue = text.substr(start, m_pos-start);
}

/*!
 * \brief Return the saved value for the token, converted to an int if possible
 *
 * This function will throw an exception if the value stored can't be converted to int.
 * It is up to the caller to either check what value type is stored before calling, or
 * handling the exception.
 *
 * The possible exceptions are std::invalid_argument if a string value can't be
 * converted to an int, or std::out_of_range if the resulting value couldn't be
 * stored in an int on the current platform.
 *
 * If the value stored is a floating point value, conversion is done by truncating
 * the number, i.e. discarding everything after the decimal point.
 *
 * \return The int value
 */
int lexer::int_value() const
{
    if (!vValue)
        return -1;
    switch (vValue->index()){
    case 0:
        return std::get<int>(vValue.value());
    case 1:
        return std::floor(std::get<double>(vValue.value()));
    case 2:
        return std::stoi(std::get<std::string>(vValue.value()));
    }
    return -1;
}

/*!
 * \brief Return the saved value for the token, converted to a double if possible
 *
 * This function will throw an exception if the value stored can't be converted to double.
 * It is up to the caller to either check what value type is stored before calling, or
 * handling the exception.
 *
 * The possible exceptions are std::invalid_argument if a string value can't be
 * converted to a double, or std::out_of_range if the resulting value couldn't be
 * stored in a double on the current platform.
 *
 * \return The double value
 */
double lexer::double_value() const
{
    if (!vValue)
        return -1;
    switch (vValue->index()){
    case 0:
        return std::get<int>(vValue.value());
    case 1:
        return std::get<double>(vValue.value());
    case 2:
        return std::stod(std::get<std::string>(vValue.value()));
    }
    return -1;
}

/*!
 * \brief Return the saved value for the token, converted to a string if needed
 * \return String representation of the matched value
 */
std::string lexer::string_value() const
{
    if (!vValue)
        return "";
    switch (vValue->index()){
    case 0:
        return std::to_string(std::get<int>(vValue.value()));
    case 1:
        return std::to_string(std::get<double>(vValue.value()));
    case 2:
        return std::get<std::string>(vValue.value());
    }
    return "";
}

/*!
 * \brief Peek ahead (normally ignoring whitespace) and return the next parsable character
 * \param acceptWhitespace Don't ignore whitespace if set to true
 * \return The first found character, or EOF if no more characters exist.
 */
char lexer::peek(bool acceptWhitespace) const
{
    if (acceptWhitespace){
        if (text.length() > m_pos){
            return text.at(m_pos);
        } else {
            return EOF;
        }
    }
    int p = m_pos;
    while (text.length() > p){
        if (text.at(p) == ' ' || text.at(p) == '\t'){
            p++;
        } else {
            return text.at(p);
        }
    }
    return EOF;
}

/*!
 * \brief Grabs an integral number from the text
 *
 * The position we are matching at in the text is advanced until after the number.
 *
 * \param offset The position where the number starts
 * \return String containing the matched number
 */
std::string lexer::grabInt(int offset)
{
    int start = m_pos+offset;
    while (m_pos < text.size() && std::isdigit(peek())){
        m_pos++;
    }
    std::string r = text.substr(start, m_pos-start);
    r.erase(std::remove_if(r.begin(), r.end(), [](const char &c){
                return c==' ' || c=='\t';}), r.end());
    return r;
}

/*!
 * \brief Tries to find a number containing the type of a given G och M code
 * \param token The current matched token type
 * \return Returns either the value given in token, or Token::unknown_code if matching failed
 */
int lexer::getCode(int token)
{
    std::string num;
    try {
        if (!std::isdigit(peek())){
            m_last_error = {error::Lexing, m_line, pos(), "Can't extract code name on \""
                                                              + text.substr(pos(), text.find('\n', m_pos))};
            return Token::unknown_code;
        }
        num = grabInt();
        vValue = std::stoi(num);
        return token;
    } catch (std::invalid_argument) {
        vValue = num;
        m_last_error = {error::Lexing, m_line, pos(), "Code name illegal on \""
                                                          + text.substr(pos(), text.find('\n', m_pos))};
        return Token::unknown_code;
    }
}

/*!
 * \brief Helper function to peek ahead if a letter string matches a known function
 *
 * If a matching function is found, the position in the text we are parsing is advanced until
 * the position after the full function name.
 *
 * \param next The current character matching works on
 * \return Either the matched function string or an empty string, if no function matches
 */
std::string lexer::checkML(char next)
{
    if (std::isalpha(next) && m_pos < text.size() && std::isalpha(text.at(m_pos))){
        int start = m_pos-1;
        int c_pos = m_pos;
        while (c_pos < text.size() && std::isalpha(text.at(c_pos))){
            c_pos++;
        }
        std::string ml = text.substr(start, c_pos-start);
        std::transform(ml.begin(), ml.end(), ml.begin(), ::toupper);
        if (multiletter.contains(ml)){
            m_pos = c_pos;
            return ml;
        } else {
            return std::string();
        }
    }
    return std::string();
}

};
