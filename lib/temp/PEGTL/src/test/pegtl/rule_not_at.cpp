// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      void unit_test()
      {
         verify_analyze< not_at< eof > >( __LINE__, __FILE__, false, false );
         verify_analyze< not_at< any > >( __LINE__, __FILE__, false, false );

         verify_rule< not_at< eof > >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< not_at< eof > >( __LINE__, __FILE__, " ", result_type::SUCCESS, 1 );
         verify_rule< not_at< any > >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );
         verify_rule< not_at< any > >( __LINE__, __FILE__, "a", result_type::LOCAL_FAILURE, 1 );
         verify_rule< not_at< any > >( __LINE__, __FILE__, "aa", result_type::LOCAL_FAILURE, 2 );
         verify_rule< not_at< any > >( __LINE__, __FILE__, "aaaa", result_type::LOCAL_FAILURE, 4 );
         verify_rule< must< not_at< alpha > > >( __LINE__, __FILE__, "a", result_type::GLOBAL_FAILURE, 1 );
         verify_rule< must< not_at< alpha, alpha > > >( __LINE__, __FILE__, "aa1", result_type::GLOBAL_FAILURE, 3 );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
