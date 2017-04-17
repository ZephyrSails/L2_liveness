# Migration Guide

## From 1.y to 2.z

Some of the [changes](Changelog.md#200) for version 2.0.0 require modifications to any source using the PEGTL.
The good news is that the semantics of all parsing rules has remained unchanged.
Existing grammars will continue to work as before once the following list of naming and namespace related changes is taken into account.

* Everything in the PEGTL is now in namespace `tao::pegtl`.
* The file name extensions were changed from `.hh` to `.hpp`.
* The main include file is now `<tao/pegtl.hpp>`, all other include files are `<tao/pegtl/*.hpp>`.
* The [parse functions](Parser-Reference.md#parser-functions) and their naming were changed.
* The string macros have been renamed from `pegtl_(i)string_t` to `TAOCPP_PEGTL_(I)STRING`.
* The first argument to actions' `apply()`-methods is now of type `tao::pegtl::action_input`.

For flexibility and future compatibility it is recommended to "template over" the first argument to `apply()` as shown in [Actions and States](Actions-and-States.md#actions).

Most of the other changes for version 2.0.0 extend or optimise the PEGTL without breaking compatibility.
Only applications integrated more tightly with the PEGTL might require some changes.

Please contact the authors at `taocpp(at)icemx.net` for any further questions when updating the PEGTL.

## From 0.x to 1.y or 2.z

There were [many important changes](Changelog.md#100) leading up to version 1.0.0.
Please contact the authors at `taocpp(at)icemx.net` for any further questions when updating the PEGTL.

Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
