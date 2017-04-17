# Actions and States

Parsing, i.e. matching an input with a grammar rule, by itself only indicates whether (a portion of the input) is valid according to the grammar.
In order to do something useful with the input, it is usually necessary to attach user-defined *actions* to one or more rules.
An action is *applied* whenever its *anchor point*, i.e. the rule to which the action is attached, succeeds.
Applying an action means that its static `apply()` or `apply0()`-method is called.
The first argument to an action application is always an instance that represents the portion of the input consumed by the successful match of the rule.

## Contents

* [Actions](#actions)
* [States](#states)
* [Action Specialisation](#action-specialisation)
* [Changing Actions](#changing-actions)
* [Legacy Actions](#legacy-actions)

## Actions

Actions are implemented as static `apply()` or `apply0()`-method of specialisations of custom class templates (which is not quite as difficult as it sounds).
First the default- or base-case of the action class template has to be defined:

```c++
template< typename Rule >
struct my_actions
   : tao::pegtl::nothing< Rule > {};
```

Inheriting from `tao::pegtl::nothing< Rule >` indicates to the PEGTL that no action is attached to `Rule`, i.e. that no `apply()` or `apply0()`-method should be called for successful matches of `Rule`.

To attach an action to `Rule`, this class template has to be specialised for `Rule` with two important properties.

1. The specialisation *must not* inherit from `tao::pegtl::nothing< Rule >`.

2. An *appropriate* static `apply()` or `apply0()`-method has to be implemented.

The `apply()`-method has to take a const-reference to an instance of an input class as first argument.

TODO: Document guaranteed interface of that input class.

As mentioned above, this input contains references to the matched portion of the input; its `line()` and `byte_in_line()` indicate the line number and (byte) offset of where the rule succeeded in the input.

```c++
template<> struct my_actions< tao::pegtl::plus< tao::pegtl::digit > >
{
   template< typename Input >
   static void apply( const Input & in )
   {
      // Called whenever a call to tao::pegtl::plus< tao::pegtl::digit >
      // in the grammar succeeds. The argument named 'in' represents the
      // matched part of the input.
   }
}
```

Alternatively, in cases where the matched part of the input is not required, the action method can be named `apply0()` instead of `apply()`.
This will suppress the first argument, the matched input, which allows for some optimisations to be applied.

```c++
template<> struct my_actions< tao::pegtl::plus< tao::pegtl::alpha > >
{
   static void apply0()
   {
      // Called whenever a call to tao::pegtl::plus< tao::pegtl::alpha >
      // in the grammar succeeds.
   }
}
```

The PEGTL will auto-detect whether a viable `apply0()`-method exists and will prefer it over `apply()`, however it is recommended to implement only either one of the two functions as future versions might flag the existence of both as error.

Actions often need to store and/or reference portions of the input for after the parsing run, for example when an abstract syntax tree is generated.
Some of the syntax tree nodes will contain portions of the input, for example for a variable name in a script language that needs to be stored in the syntax tree just as it occurs in the input data.

The **default safe choice** is to copy the matched portions of the input data that are passed to an action by storing a deep copy of the data as `std::string`, as obtained by the input class' `string()` method, in the data structures built while parsing.

## States

In most applications, the actions also need some kind of data or user-defined (parser/action) *state* to operate on.
Since the `apply()` and `apply0()`-methods are `static`, they do not have an instance of the class of which they are a member function available for this purpose.
Therefore the *state(s)* are an arbitrary collection of objects that are

* passed by the user as additional arguments to the `parse()`-function that starts a parsing run, and then

* passed by the PEGTL as additional arguments to all actions' `apply()` or `apply0()`-method.

In other words, the additional arguments to the `apply()` and `apply0()`-method can be chosen freely, however **all** actions **must** accept the same argument list since they are **all** called with the same arguments.

For example, in a practical grammar the example from above might use a second argument to store the parsed sequence of digits somewhere.

```c++
template<> struct my_actions< tao::pegtl::plus< tao::pegtl::digit > >
{
   template< typename Input >
   static void apply( const Input & in,
                      std::vector< std::string > & digit_strings )
   {
      digit_strings.push_back( in.string() );
   }
}
```

If we then assume that our grammar `my_grammar` contains the rule `tao::pegtl::plus< tao::pegtl::digit >` somewhere, we can use

```c++
const std::string parsed_data = ...;
std::vector< std::string > digit_strings;
tao::pegtl::parse_string< my_grammar,
                     my_actions >( parsed_data,
                                  "data-source-name",
                                  digit_strings );
```

to collect all `digit_strings` that were detected by the grammar, i.e. the vector will contain one string for every time that the `tao::pegtl::plus< tao::pegtl::digit >` rule was matched against the input.

Since the `parse()`-functions are variadic function templates, an arbitrary sequence of state arguments can be used.

## Action Specialisation

The rule class for which the action class template is specialised *must* exactly match how the rule is defined and referenced in the grammar.
For example given the rule

```c++
struct foo : tao::pegtl::plus< tao::pegtl::one< '*' > > {};
```

an action class template can be specialised for `foo` or for `tao::pegtl::one< '*' >`, but *not* for `tao::pegtl::plus< tao::pegtl::one< '*' > >` because that is not the rule class name whose `match()`-method is called.

(The method is called on class `foo`, which happens to inherit `match()` from `tao::pegtl::plus< tao::pegtl::one< '*' > >`, however base classes are not taken into consideration by the C++ language when choosing a specialisation.)

To then use these actions in a parsing run, simply pass them as additional template parameter to one of the parser functions defined in `<tao/pegtl/parser.hpp>`, e.g. `parse_string()`.

```c++
tao::pegtl::parse_string< my_grammar, my_actions >( ... );
```

## Changing Actions

Within a grammar, the action class template can be changed, enabled or disabled using the `action<>`, `enable<>` and `disable<>` rules.

The following two lines effectively do the same thing, namely parse with `my_grammar` as top-level parsing rule without invoking actions (unless actions are enabled again somewhere within the grammar).

```c++
tao::pegtl::parse_string< my_grammar >( ... );
tao::pegtl::parse_string< tao::pegtl::disable< my_grammar >, my_actions >( ... );
```

Similarly the following two lines both start parsing `my_grammar` with `my_actions` (again with the caveat that something might change somewhere in the grammar).

```c++
tao::pegtl::parse_string< my_grammar, my_actions >( ... );
tao::pegtl::parse_string< tao::pegtl::action< my_actions, my_grammar > >( ... );
```

As usual this applies not just to `parse_string()`, but equally to all parser functions defined in `<tao/pegtl/parser.hpp>` or the `parse()` member methods of the parser classes documented in [Parser Reference](Parser-Reference.md).

In other words, `enable<>` and `disable<>` behave just like `seq<>` but enable or disable the calling of actions. `action<>` changes the active action class template, which must be supplied as first template parameter to `action<>`.

Note that `action<>` does *not* implicitly enable actions when they were previously explicitly disabled.

User-defined parsing rules can use `action<>`, `enable<>` and `disable<>` just like any other combinator rules, for example to disable actions in LISP-style comments:

```c++
struct comment
   : tao::pegtl::seq< tao::pegtl::one< '#' >, tao::pegtl::disable< cons_list > > {};
```

This also allows using the same rules multiple times with different actions within the grammar.

## Changing States

See the [page on Switching Style](Switching-Style.md).

## Legacy Actions

See the [section on legacy-style action rules](Rule-Reference.md#action-rules).

Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
