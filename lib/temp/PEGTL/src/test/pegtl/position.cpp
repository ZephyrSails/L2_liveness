// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Rule >
      void test_matches_lf()
      {
         static const std::string s1 = "\n";

         memory_input<> i1( { 0, 1, 0, s1.data() }, s1.data() + s1.size(), __FUNCTION__ );

         TAOCPP_PEGTL_TEST_ASSERT( parse< Rule >( i1 ) );
         TAOCPP_PEGTL_TEST_ASSERT( i1.line() == 2 );
         TAOCPP_PEGTL_TEST_ASSERT( i1.byte_in_line() == 0 );
      }

      template< typename Rule >
      void test_matches_other( const std::string& s2 )
      {
         TAOCPP_PEGTL_TEST_ASSERT( s2.size() == 1 );

         memory_input<> i2( { 0, 1, 0, s2.data() }, s2.data() + s2.size(), __FUNCTION__ );

         TAOCPP_PEGTL_TEST_ASSERT( parse< Rule >( i2 ) );
         TAOCPP_PEGTL_TEST_ASSERT( i2.line() == 1 );
         TAOCPP_PEGTL_TEST_ASSERT( i2.byte_in_line() == 1 );
      }

      template< typename Rule >
      void test_mismatch( const std::string& s3 )
      {
         TAOCPP_PEGTL_TEST_ASSERT( s3.size() == 1 );

         memory_input<> i3( { 0, 1, 0, s3.data() }, s3.data() + s3.size(), __FUNCTION__ );

         TAOCPP_PEGTL_TEST_ASSERT( !parse< Rule >( i3 ) );
         TAOCPP_PEGTL_TEST_ASSERT( i3.line() == 1 );
         TAOCPP_PEGTL_TEST_ASSERT( i3.byte_in_line() == 0 );
      }

      struct outer_grammar
         : must< two< 'a' >, two< 'b' >, two< 'c' >, eof >
      {
      };

      struct inner_grammar
         : must< one< 'd' >, two< 'e' >, eof >
      {
      };

      template< typename Rule >
      struct outer_action
         : nothing< Rule >
      {
      };

      template<>
      struct outer_action< two< 'b' > >
      {
         template< typename Input >
         static void apply( const Input& oi )
         {
            const auto p = oi.position();
            TAOCPP_PEGTL_TEST_ASSERT( p.source == "outer" );
            TAOCPP_PEGTL_TEST_ASSERT( p.byte == 2 );
            TAOCPP_PEGTL_TEST_ASSERT( p.line == 1 );
            TAOCPP_PEGTL_TEST_ASSERT( p.byte_in_line == 2 );
            memory_input<> in( "dFF", "inner" );
            parse_nested< inner_grammar >( oi, in );
         }
      };

      void test_nested()
      {
         try {
            memory_input<> oi( "aabbcc", "outer" );
            parse< outer_grammar, outer_action >( oi );
         }
         catch( const parse_error& e ) {
            TAOCPP_PEGTL_TEST_ASSERT( e.positions.size() == 2 );
            TAOCPP_PEGTL_TEST_ASSERT( e.positions[ 0 ].source == "inner" );
            TAOCPP_PEGTL_TEST_ASSERT( e.positions[ 0 ].byte == 1 );
            TAOCPP_PEGTL_TEST_ASSERT( e.positions[ 0 ].line == 1 );
            TAOCPP_PEGTL_TEST_ASSERT( e.positions[ 0 ].byte_in_line == 1 );
            TAOCPP_PEGTL_TEST_ASSERT( e.positions[ 1 ].source == "outer" );
            TAOCPP_PEGTL_TEST_ASSERT( e.positions[ 1 ].byte == 2 );
            TAOCPP_PEGTL_TEST_ASSERT( e.positions[ 1 ].line == 1 );
            TAOCPP_PEGTL_TEST_ASSERT( e.positions[ 1 ].byte_in_line == 2 );
         }
      }

      void unit_test()
      {
         test_matches_lf< any >();
         test_matches_other< any >( " " );

         test_matches_lf< one< '\n' > >();
         test_mismatch< one< '\n' > >( " " );

         test_matches_lf< one< ' ', '\n' > >();
         test_matches_other< one< ' ', '\n' > >( " " );

         test_matches_lf< one< ' ', '\n', 'b' > >();
         test_matches_other< one< ' ', '\n', 'b' > >( " " );

         test_matches_lf< string< '\n' > >();
         test_mismatch< string< '\n' > >( " " );

         test_matches_other< string< ' ' > >( " " );
         test_mismatch< string< ' ' > >( "\n" );

         test_matches_lf< range< 8, 33 > >();
         test_matches_other< range< 8, 33 > >( " " );

         test_mismatch< range< 11, 30 > >( "\n" );
         test_mismatch< range< 11, 30 > >( " " );

         test_matches_lf< not_range< 20, 30 > >();
         test_matches_other< not_range< 20, 30 > >( " " );

         test_mismatch< not_range< 5, 35 > >( "\n" );
         test_mismatch< not_range< 5, 35 > >( " " );

         test_matches_lf< ranges< 'a', 'z', 8, 33, 'A', 'Z' > >();
         test_matches_other< ranges< 'a', 'z', 8, 33, 'A', 'Z' > >( "N" );
         test_mismatch< ranges< 'a', 'z', 8, 33, 'A', 'Z' > >( "9" );

         test_matches_lf< ranges< 'a', 'z', 'A', 'Z', '\n' > >();
         test_matches_other< ranges< 'a', 'z', 'A', 'Z', '\n' > >( "P" );
         test_mismatch< ranges< 'a', 'z', 'A', 'Z', '\n' > >( "8" );

         test_nested();
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
