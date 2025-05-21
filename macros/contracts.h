#ifndef CONTRACTS_H
#define CONTRACTS_H

// Implement [[expects]] and [[ensures]] for compilers that support them
#if defined(__cpp_contracts) && __cpp_contracts >= 201907L
#define EXPECTS(cond) [[expects: cond]]
#define ENSURES(cond) [[ensures: cond]]
#elif defined(__has_cpp_attribute)
#if __has_cpp_attribute(expects)
#define EXPECTS(cond) [[expects: cond]]
#else
#define EXPECTS(cond)
#endif
#if __has_cpp_attribute(ensures)
#define ENSURES(cond) [[ensures: cond]]
#else
#define ENSURES(cond)
#endif
#else
#define EXPECTS(cond)
#define ENSURES(cond)
#endif

#endif // CONTRACTS_H
