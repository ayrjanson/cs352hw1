#pragma once

// expression language

#include <cassert>
#include <functional>
#include <memory>
#include <ostream>
#include <utility>

/// Crashes with an "unimplemented" error, syntactically returning 
/// a value of type N.
template<typename N>
N unimplemented() {
    assert(!"unimplemented");
}

/// All expressions of type T
template<typename T>
class expr {
public:
    /// evaluates the expression to a C++ value
    virtual T eval() const = 0;
    
    /// prints the expression
    virtual void print(std::ostream&) const = 0;
    
    /// makes a deep copy of this expression node.
    /// the clone should be deleted by the caller
    virtual expr* clone() const = 0;

    // ensures that subclasses are deleted properly
    virtual ~expr() = default;
};

/// Prints an arbitrary expression
template<typename T>
std::ostream& operator<< (std::ostream& out, const expr<T>& expr) {
    expr.print(out);
    return out;
}

/// Constants of type T
template<typename T>
class const_expr : public expr<T> {
    /// The underlying value
    T val;

public:
    const_expr(const T& v) 
    : val(unimplemented<T>()) {}

    T eval() const override {
        return unimplemented<T>();
    }

    void print(std::ostream& out) const override {
        unimplemented<void>();
    }

    const_expr* clone() const override {
        return unimplemented<const_expr*>();
    }
};

/// Binary operators returning type T, with left and right operands of types A & B.
template<typename T, typename A, typename B>
class bin_op_expr : public expr<T> {
    /// The C++ function;
    /// uses the std::function type to store a function with parameters of types 
    /// A & B and return type T
    std::function<T(A,B)> fn;
    /// The name of the operator
    std::string name;
    /// The left operand
    std::unique_ptr<expr<A>> left_arg;
    /// The right operand
    std::unique_ptr<expr<B>> right_arg;

public:
    /// Constructs a binary operator expression.
    /// Will delete the passed-in pointers
    /// The template parameter allows any object which can be stored in a 
    /// std::function to be used to construct the function
    template<typename F>
    bin_op_expr(
        F&& f, const std::string& n, expr<A>* l, expr<B>* r)
    : fn(f), name(n), left_arg(l), right_arg(r) {}

    bin_op_expr(const bin_op_expr& o)
    : fn(o.fn), name(o.name), left_arg(o.left_arg->clone()), 
      right_arg(o.right_arg->clone()) {}
    
    bin_op_expr& operator= (const bin_op_expr& o) {
        if ( &o == this ) return *this;
        
        fn = o.fn;
        name = o.name;
        // there's no assignment operator from raw pointer to unique_ptr
        // `reset` replaces the contained pointer, though, after deleting
        // the previously-contained pointer
        left_arg.reset(o.left_arg->clone());
        right_arg.reset(o.right_arg->clone());
        
        return *this;
    }

    // We can use the default move-construction and -assignment operators for 
    // unique_ptr, the = default; syntax tells the C++ compiler to generate 
    // the methods it would have anyway. We needed to write our own copy- 
    // operators because unique_ptr doesn't have copy-operators.

    bin_op_expr(bin_op_expr&&) = default;
    bin_op_expr& operator= (bin_op_expr&&) = default;

    T eval() const override {
        return fn(left_arg->eval(), right_arg->eval());
    }

    void print(std::ostream& out) const override {
        out << "(" << *left_arg << " " << name << " " << *right_arg << ")";
    }

    bin_op_expr* clone() const override {
        return new bin_op_expr(fn, name, left_arg->clone(), right_arg->clone());
    }
};

/// Conditional expressions returning type T
template<typename T>
class if_expr : public expr<T> {
    /// The conditional expression
    expr<bool>* cond;
    /// The expression to evaluate to if the condition is true
    expr<T>* true_branch;
    /// The expression to evaluate to if the condition is false
    expr<T>* false_branch;

public:
    if_expr(expr<bool>* c, expr<T>* t, expr<T>* f)
    : cond(unimplemented<expr<bool>*>()), 
      true_branch(unimplemented<expr<T>*>()), 
      false_branch(unimplemented<expr<T>*>()) {}

    ~if_expr() {
        unimplemented<void>();
    }

    if_expr(const if_expr& o) 
    : cond(unimplemented<expr<bool>*>()), 
      true_branch(unimplemented<expr<T>*>()), 
      false_branch(unimplemented<expr<T>*>()) {}

    if_expr& operator= (const if_expr& o) {
        unimplemented<void>();

        return *this;
    }

    if_expr(if_expr&& o) 
    : cond(unimplemented<expr<bool>*>()), 
      true_branch(unimplemented<expr<T>*>()), 
      false_branch(unimplemented<expr<T>*>()) {
        unimplemented<void>();
    }

    if_expr& operator= (if_expr&& o) {
        unimplemented<void>();

        return *this;
    }

    T eval() const override {
        return unimplemented<T>();
    }

    void print(std::ostream& out) const override {
        unimplemented<void>();
    }

    if_expr* clone() const override {
        return unimplemented<if_expr*>();
    }
};