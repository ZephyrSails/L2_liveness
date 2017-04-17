// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/parse_arg.hpp>

namespace pegtl = tao::TAOCPP_PEGTL_NAMESPACE;

namespace hello
{
   // clang-format off
   struct prefix : pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' > {};
   struct name : pegtl::plus< pegtl::alpha > {};
   struct grammar : pegtl::must< prefix, name, pegtl::one< '!' >, pegtl::eof > {};
   // clang-format on

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {
   };

   template<>
   struct action< name >
   {
      template< typename Input >
      static void apply( const Input& in, std::string& name )
      {
         name = in.string();
      }
   };

}  // namespace hello

int main( int argc, char** argv )
{
   if( argc > 1 ) {
      std::string name;
      pegtl::parse_arg< hello::grammar, hello::action >( 1, argv, name );
      std::cout << "Good bye, " << name << "!" << std::endl;
   }
}
