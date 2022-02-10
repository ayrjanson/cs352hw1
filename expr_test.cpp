#include "expr.hpp"

#include <iostream>
#include <string>
#include <utility>

/// wrapper function for addition
int plus(int a, int b) { return a + b; }

/// wrapper function for equality
bool equals(int a, int b) { return a == b; }
int mult(int a, int b) { return a * b;}

int main() {
    // constant expressions
    auto two = new const_expr<int>(2);
    std::cout << *two << " = " << two->eval() << std::endl;

    // binary operators
    // note that because the expr nodes take ownership of their 
    // constructor arguments I can create these concisely in-place
    auto cond = new bin_op_expr<bool, int, int>(
        equals, "==",
        new bin_op_expr<int, int, int>(
            plus, "+",
            two,
            two->clone()
        ),
        new const_expr<int>(4)
    );
    std::cout << *cond << " = " << (cond->eval() ? "true" : "false") << std::endl;

    // more complex if-expression
    // note that this top-level expression is stack-allocated, so we don't 
    // need to delete it when we're done with it, and root's destructor will 
    // delete all the other expressions we've created so far.
    if_expr<std::string> root = {
        cond,
        new const_expr<std::string>("correct"),
        new const_expr<std::string>("incorrect")
    };
    std::cout << root << "\n = " << root.eval() << std::endl;

    // TODO: write your own expression containing at least 6 expression nodes, 
    // and build the expression object and print it below, similar to the 
    // `root` example above. You may nest the subexpressions or put them in 
    // separate variables, as you will

    // Plug these into each other to create the tree - good luck bestie
    auto first = new const_expr<int>(7);
    auto second = new const_expr<int>(6);

    auto first_expr = new bin_op_expr<bool, int, int>(
        equals, "==",
        new bin_op_expr<int, int, int>(
            mult, "*",
            first,
            second
        ),
        new const_expr<int>(42)
    );

    if_expr<std::string>* second_expr = new if_expr<std::string> {
        second_expr,
        new const_expr<std::string>("correct"),
        new const_expr<std::string>("incorrect")
    };

    std::cout << *first_expr << "\n = " << first_expr->eval() << std::endl;
    std::cout << *second_expr << "\n = " << second_expr->eval() << std::endl;
    delete second_expr;
}