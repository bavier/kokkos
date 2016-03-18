/*
//@HEADER
// ************************************************************************
// 
//                        Kokkos v. 2.0
//              Copyright (2014) Sandia Corporation
// 
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact  H. Carter Edwards (hcedwar@sandia.gov)
// 
// ************************************************************************
//@HEADER
*/

/// \file Kokkos_DynRankView.hpp
/// \brief Declaration and definition of Kokkos::Experimental::DynRankView.
///
/// This header file declares and defines Kokkos::Experimental::DynRankView and its
/// related nonmember functions.

#ifndef KOKKOS_DYNRANKVIEW_HPP
#define KOKKOS_DYNRANKVIEW_HPP

#include <Kokkos_Core.hpp>
#include <impl/Kokkos_Error.hpp>
#include <type_traits>

namespace Kokkos {
namespace Experimental {

/* \class DynRankView
 * \brief Container that creates a Kokkos view with runtime rank. 
 *   Essentially this is a rank 8 view with that wraps the access operators
 *   to yield the functionality of a view with rank that varies. 
 */

template< typename DataType , class ... Properties >
class DynRankView : private View< DataType********, Properties... >
{
  static_assert( !std::is_array<DataType>::value && !std::is_pointer<DataType>::value , "Cannot template DynRankView with array or pointer datatype - must be pod" );

public: 
  using view_type = View< DataType******** , Properties...>;
  using reference_type = typename view_type::reference_type; 


  template< class D, class ... P , class ... Args >
  friend
  KOKKOS_INLINE_FUNCTION
  DynRankView< D , P... >
  subview( const DynRankView< D, P... > & src , Args ... args );

  template< class D, class ... P , class ... Args >
  friend
  KOKKOS_INLINE_FUNCTION
  DynRankView< D , P... >
  dsubview( const DynRankView< D, P... > & src , Args ... args );

private: 
  template < class , class ... > friend class DynRankView ;
  unsigned m_rank;

public:
  KOKKOS_INLINE_FUNCTION
  view_type & DownCast() const { return static_cast< view_type & > (*this); }
  KOKKOS_INLINE_FUNCTION
  const view_type & ConstDownCast() const { return static_cast< const view_type & > (*this); }

  typedef ViewTraits< DataType , Properties ... > traits ;

  typedef typename traits::execution_space execution_space;
  typedef typename traits::memory_space memory_space;

  typedef typename traits::size_type size_type;

  typedef typename traits::data_type data_type;
  typedef typename traits::const_data_type const_data_type;
  typedef typename traits::non_const_data_type non_const_data_type;

  typedef typename traits::host_mirror_space host_mirror_space;

  /** \brief  Compatible view of array of scalar types */
  typedef DynRankView< typename traits::scalar_array_type ,
                       typename traits::array_layout ,
                typename traits::device_type ,
                typename traits::memory_traits >
    array_type ;

  /** \brief  Compatible view of const data type */
  typedef DynRankView< typename traits::const_data_type ,
                typename traits::array_layout ,
                typename traits::device_type ,
                typename traits::memory_traits >
    const_type ;

  /** \brief  Compatible view of non-const data type */
  typedef DynRankView< typename traits::non_const_data_type ,
                typename traits::array_layout ,
                typename traits::device_type ,
                typename traits::memory_traits >
    non_const_type ;

  /** \brief  Compatible HostMirror view */
  typedef DynRankView< typename traits::non_const_data_type ,
                typename traits::array_layout ,
                typename traits::host_mirror_space >
    HostMirror ;

  //----------------------------------------
  // Domain rank and extents

  KOKKOS_INLINE_FUNCTION
  DynRankView() : view_type() , m_rank(0) {}

  KOKKOS_INLINE_FUNCTION
  constexpr unsigned rank() const { return m_rank; }

  using view_type::extent; 
  using view_type::extent_int; 
  using view_type::layout;
  using view_type::dimension;
  using view_type::size;
  using view_type::stride;

  using pointer_type = typename view_type::pointer_type;
  using view_type::reference_type_is_lvalue_reference;
  using view_type::span;
  using view_type::capacity;
  using view_type::span_is_contiguous;
  using view_type::data;
  using view_type::implementation_map;

  using view_type::is_contiguous;
  using view_type::ptr_on_device;

  //Deprecated, remove soon (add for test)
  using view_type::dimension_0;
  using view_type::dimension_1;
  using view_type::dimension_2;
  using view_type::dimension_3;
  using view_type::dimension_4;
  using view_type::dimension_5;
  using view_type::dimension_6;
  using view_type::dimension_7;
  using view_type::stride_0;
  using view_type::stride_1;
  using view_type::stride_2;
  using view_type::stride_3;
  using view_type::stride_4;
  using view_type::stride_5;
  using view_type::stride_6;
  using view_type::stride_7;

  //operators ()
  // Rank 0
  KOKKOS_INLINE_FUNCTION
  reference_type operator()() const
    { return view_type::operator()(0,0,0,0,0,0,0,0); }
  
  // Rank 1
  template< typename iType >
  KOKKOS_INLINE_FUNCTION
  reference_type operator[](const iType i0) const
    { return view_type::operator[](i0,0,0,0,0,0,0,0); }

  template< typename iType >
  KOKKOS_INLINE_FUNCTION
  reference_type operator()(const iType i0 ) const 
    { return view_type::operator()(i0,0,0,0,0,0,0,0); }

  // Rank 2
  template< typename iType0 , typename iType1 >
  KOKKOS_INLINE_FUNCTION
  reference_type operator()(const iType0 i0 , const iType1 i1 ) const 
    { return view_type::operator()(i0,i1,0,0,0,0,0,0); }

  // Rank 3
  template< typename iType0 , typename iType1 , typename iType2 >
  KOKKOS_INLINE_FUNCTION
  reference_type operator()(const iType0 i0 , const iType1 i1 , const iType2 i2 ) const 
    { return view_type::operator()(i0,i1,i2,0,0,0,0,0); }

  // Rank 4
  template< typename iType0 , typename iType1 , typename iType2 , typename iType3 >
  KOKKOS_INLINE_FUNCTION
  reference_type operator()(const iType0 i0 , const iType1 i1 , const iType2 i2 , const iType3 i3 ) const 
    { return view_type::operator()(i0,i1,i2,i3,0,0,0,0); }

  // Rank 5
  template< typename iType0 , typename iType1 , typename iType2 , typename iType3, typename iType4 >
  KOKKOS_INLINE_FUNCTION
  reference_type operator()(const iType0 i0 , const iType1 i1 , const iType2 i2 , const iType3 i3 , const iType4 i4 ) const 
    { return view_type::operator()(i0,i1,i2,i3,i4,0,0,0); }

  // Rank 6
  template< typename iType0 , typename iType1 , typename iType2 , typename iType3, typename iType4 , typename iType5 >
  KOKKOS_INLINE_FUNCTION
  reference_type operator()(const iType0 i0 , const iType1 i1 , const iType2 i2 , const iType3 i3 , const iType4 i4 , const iType5 i5 ) const 
    { return view_type::operator()(i0,i1,i2,i3,i4,i5,0,0); }

  // Rank 7
  template< typename iType0 , typename iType1 , typename iType2 , typename iType3, typename iType4 , typename iType5 , typename iType6 >
  KOKKOS_INLINE_FUNCTION
  reference_type operator()(const iType0 i0 , const iType1 i1 , const iType2 i2 , const iType3 i3 , const iType4 i4 , const iType5 i5 , const iType6 i6 ) const 
    { return view_type::operator()(i0,i1,i2,i3,i4,i5,i6,0); }

  // Rank 8
  template< typename iType0 , typename iType1 , typename iType2 , typename iType3, typename iType4 , typename iType5 , typename iType6 , typename iType7 >
  KOKKOS_INLINE_FUNCTION
  reference_type operator()(const iType0 i0 , const iType1 i1 , const iType2 i2 , const iType3 i3 , const iType4 i4 , const iType5 i5 , const iType6 i6 , const iType7 i7 ) const 
    { return view_type::operator()(i0,i1,i2,i3,i4,i5,i6,i7); }


  //----------------------------------------
  // Standard constructor, destructor, and assignment operators... 

  KOKKOS_INLINE_FUNCTION
  ~DynRankView() {}

  KOKKOS_INLINE_FUNCTION
  DynRankView( const DynRankView & ) = default ;

  KOKKOS_INLINE_FUNCTION
  DynRankView( DynRankView && ) = default ;

  KOKKOS_INLINE_FUNCTION
  DynRankView & operator = ( const DynRankView & ) = default ;

  KOKKOS_INLINE_FUNCTION
  DynRankView & operator = ( DynRankView && ) = default ;

  //----------------------------------------
  // Compatible view copy constructor and assignment
  // may assign unmanaged from managed.

  template< class RT , class ... RP >
  KOKKOS_INLINE_FUNCTION
  DynRankView( const DynRankView<RT,RP...> & rhs )
    : view_type( rhs.ConstDownCast() )  
    , m_rank(rhs.m_rank)
    {}

  template< class RT , class ... RP >
  KOKKOS_INLINE_FUNCTION
  DynRankView & operator = (const DynRankView<RT,RP...> & rhs )
  {
    view_type::operator = ( rhs.ConstDownCast() );
    m_rank = rhs.rank();
    return *this;
  }

  //----------------------------------------
  // Compatible subview constructor
  // may assign unmanaged from managed.
  //
  //Subview version of constructor accepting a rank 8 view
  template< class RT , class ... RP >
  KOKKOS_INLINE_FUNCTION
  DynRankView( const Kokkos::Experimental::View<RT********,RP...> & rhs , unsigned RankVal )
    : view_type( rhs ) 
    , m_rank(RankVal)
    {}
/*
    { 
      m_rank = RankVal; 
      view_type::operator = (rhs); 
      std::cout << "DRV ctor 1 and RankVal "<< RankVal << std::endl;
    }
*/

  //----------------------------------------
  // Allocation tracking properties

  using view_type::use_count;
  using view_type::label;

  //----------------------------------------
  // Allocation according to allocation properties and array layout

  template< class ... P >
  explicit inline
  DynRankView( const Impl::ViewCtorProp< P ... > & arg_prop
      , typename std::enable_if< ! Impl::ViewCtorProp< P... >::has_pointer
                               , typename traits::array_layout
                               >::type const & arg_layout
      )
      : view_type( arg_prop 
                 , typename traits::array_layout
                   ( arg_layout.dimension[0] != 0 ? arg_layout.dimension[0] : 1 
                   , arg_layout.dimension[1] != 0 ? arg_layout.dimension[1] : 1 
                   , arg_layout.dimension[2] != 0 ? arg_layout.dimension[2] : 1 
                   , arg_layout.dimension[3] != 0 ? arg_layout.dimension[3] : 1 
                   , arg_layout.dimension[4] != 0 ? arg_layout.dimension[4] : 1 
                   , arg_layout.dimension[5] != 0 ? arg_layout.dimension[5] : 1 
                   , arg_layout.dimension[6] != 0 ? arg_layout.dimension[6] : 1 
                   , arg_layout.dimension[7] != 0 ? arg_layout.dimension[7] : 1 
                   ) 
                 )
      , m_rank( ( arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0 && arg_layout.dimension[3] == 0 && arg_layout.dimension[2] == 0 && arg_layout.dimension[1] == 0 && arg_layout.dimension[0] == 0) ? 0 
            : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0 && arg_layout.dimension[3] == 0 && arg_layout.dimension[2] == 0 && arg_layout.dimension[1] == 0) ? 1 
            : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0 && arg_layout.dimension[3] == 0 && arg_layout.dimension[2] == 0) ? 2 
            : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0 && arg_layout.dimension[3] == 0) ? 3 
            : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0) ? 4 
            : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0) ? 5 
            : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0) ? 6 
            : ( arg_layout.dimension[7] == 0 ? 7 
            : 8 ) ) ) ) ) ) )
            )  
    {}
/*
    {  std::cout << "DRV ctor 2"<<std::endl;
       std::cout << "  arg_layout dims \n" << arg_layout.dimension[0] << " "
                                           << arg_layout.dimension[1] << " "
                                           << arg_layout.dimension[2] << " "
                                           << arg_layout.dimension[3] << " "
                                           << arg_layout.dimension[4] << " "
                                           << arg_layout.dimension[5] << " "
                                           << arg_layout.dimension[6] << " "
                                           << arg_layout.dimension[7] << std::endl;
    }
*/

//Wrappers
  template< class ... P >
  explicit KOKKOS_INLINE_FUNCTION
  DynRankView( const Impl::ViewCtorProp< P ... > & arg_prop
      , typename std::enable_if< Impl::ViewCtorProp< P... >::has_pointer
                               , typename traits::array_layout
                               >::type const & arg_layout
      )
      : view_type( arg_prop 
                , typename traits::array_layout
                   ( arg_layout.dimension[0] != 0 ? arg_layout.dimension[0] : 1 
                   , arg_layout.dimension[1] != 0 ? arg_layout.dimension[1] : 1 
                   , arg_layout.dimension[2] != 0 ? arg_layout.dimension[2] : 1 
                   , arg_layout.dimension[3] != 0 ? arg_layout.dimension[3] : 1 
                   , arg_layout.dimension[4] != 0 ? arg_layout.dimension[4] : 1 
                   , arg_layout.dimension[5] != 0 ? arg_layout.dimension[5] : 1 
                   , arg_layout.dimension[6] != 0 ? arg_layout.dimension[6] : 1 
                   , arg_layout.dimension[7] != 0 ? arg_layout.dimension[7] : 1 
                   ) 
               )
      , m_rank( 
               ( arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0 && arg_layout.dimension[3] == 0 && arg_layout.dimension[2] == 0 && arg_layout.dimension[1] == 0 && arg_layout.dimension[0] == 0) ? 0 
              : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0 && arg_layout.dimension[3] == 0 && arg_layout.dimension[2] == 0 && arg_layout.dimension[1] == 0) ? 1 
              : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0 && arg_layout.dimension[3] == 0 && arg_layout.dimension[2] == 0) ? 2 
              : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0 && arg_layout.dimension[3] == 0) ? 3 
              : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0 && arg_layout.dimension[4] == 0) ? 4 
              : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0 && arg_layout.dimension[5] == 0) ? 5 
              : ( (arg_layout.dimension[7] == 0 && arg_layout.dimension[6] == 0) ? 6 
              : ( arg_layout.dimension[7] == 0 ? 7 
              : 8 ) ) ) ) ) ) )
            )  
    {}
//    {std::cout << "DRV ctor 3"<<std::endl;}

  //----------------------------------------
  //Constructor(s)

  // Simple dimension-only layout
  template< class ... P >
  explicit inline
  DynRankView( const Impl::ViewCtorProp< P ... > & arg_prop
      , typename std::enable_if< ! Impl::ViewCtorProp< P... >::has_pointer
                               , size_t
                               >::type const arg_N0 = 0
      , const size_t arg_N1 = 0
      , const size_t arg_N2 = 0
      , const size_t arg_N3 = 0
      , const size_t arg_N4 = 0
      , const size_t arg_N5 = 0
      , const size_t arg_N6 = 0
      , const size_t arg_N7 = 0
      )
    : DynRankView( arg_prop 
    , typename traits::array_layout
          ( arg_N0 , arg_N1 , arg_N2 , arg_N3 , arg_N4 , arg_N5 , arg_N6 , arg_N7 )
      )
    {}
//    {std::cout << "DRV ctor 4"<<std::endl;}

  template< class ... P >
  explicit KOKKOS_INLINE_FUNCTION
  DynRankView( const Impl::ViewCtorProp< P ... > & arg_prop
      , typename std::enable_if< Impl::ViewCtorProp< P... >::has_pointer
                               , size_t
                               >::type const arg_N0 = 0
      , const size_t arg_N1 = 0
      , const size_t arg_N2 = 0
      , const size_t arg_N3 = 0
      , const size_t arg_N4 = 0
      , const size_t arg_N5 = 0
      , const size_t arg_N6 = 0
      , const size_t arg_N7 = 0
      )
    : DynRankView( arg_prop 
    , typename traits::array_layout
          ( arg_N0 , arg_N1 , arg_N2 , arg_N3 , arg_N4 , arg_N5 , arg_N6 , arg_N7 )
      )
    {}
//    {std::cout << "DRV ctor 5"<<std::endl;}

  // Allocate with label and layout
  template< typename Label >
  explicit inline
  DynRankView( const Label & arg_label
      , typename std::enable_if<
          Kokkos::Experimental::Impl::is_view_label<Label>::value ,
          typename traits::array_layout >::type const & arg_layout
      )
    : DynRankView( Impl::ViewCtorProp< std::string >( arg_label ) , arg_layout )
    {}
//    {std::cout << "DRV ctor 6"<<std::endl;}

  // Allocate label and layout, must disambiguate from subview constructor.
  template< typename Label >
  explicit inline
  DynRankView( const Label & arg_label
      , typename std::enable_if<
          Kokkos::Experimental::Impl::is_view_label<Label>::value ,
        const size_t >::type arg_N0 = 0
      , const size_t arg_N1 = 0
      , const size_t arg_N2 = 0
      , const size_t arg_N3 = 0
      , const size_t arg_N4 = 0
      , const size_t arg_N5 = 0
      , const size_t arg_N6 = 0
      , const size_t arg_N7 = 0
      )
    : DynRankView( Impl::ViewCtorProp< std::string >( arg_label )
    , typename traits::array_layout
          ( arg_N0 , arg_N1 , arg_N2 , arg_N3 , arg_N4 , arg_N5 , arg_N6 , arg_N7 )
          )
    {}
//    {std::cout << "DRV ctor 7 \n"<<std::endl;}

  // For backward compatibility
/*
  explicit inline
  DynRankView( const ViewAllocateWithoutInitializing & arg_prop
      , const typename traits::array_layout & arg_layout
      )
    : view_type( Impl::ViewCtorProp< std::string , Kokkos::Experimental::Impl::WithoutInitializing_t >( arg_prop.label , Kokkos::Experimental::WithoutInitializing )
          , arg_layout
          )
    //, m_rank(arg_N0 == 0 ? 0 : ( arg_N1 == 0 ? 1 : ( arg_N2 == 0 ? 2 : ( arg_N3 == 0 ? 3 : ( arg_N4 == 0 ? 4 : ( arg_N5 == 0 ? 5 : ( arg_N6 == 0 ? 6 : ( arg_N7 == 0 ? 7 : 8 ) ) ) ) ) ) ) ) //how to extract rank?
    {}
*/

  explicit inline
  DynRankView( const ViewAllocateWithoutInitializing & arg_prop
      , const size_t arg_N0 = 0
      , const size_t arg_N1 = 0
      , const size_t arg_N2 = 0
      , const size_t arg_N3 = 0
      , const size_t arg_N4 = 0
      , const size_t arg_N5 = 0
      , const size_t arg_N6 = 0
      , const size_t arg_N7 = 0
      )
    : DynRankView(Impl::ViewCtorProp< std::string , Kokkos::Experimental::Impl::WithoutInitializing_t >( arg_prop.label , Kokkos::Experimental::WithoutInitializing ), arg_N0, arg_N1, arg_N2, arg_N3, arg_N4, arg_N5, arg_N6, arg_N7 ) 
    {}
//    {std::cout << "DRV ctor 8"<<std::endl;}

  using view_type::memory_span;

  explicit KOKKOS_INLINE_FUNCTION
  DynRankView( pointer_type arg_ptr
      , const size_t arg_N0 = 0
      , const size_t arg_N1 = 0
      , const size_t arg_N2 = 0
      , const size_t arg_N3 = 0
      , const size_t arg_N4 = 0
      , const size_t arg_N5 = 0
      , const size_t arg_N6 = 0
      , const size_t arg_N7 = 0
      )
    : DynRankView( Impl::ViewCtorProp<pointer_type>(arg_ptr) , arg_N0, arg_N1, arg_N2, arg_N3, arg_N4, arg_N5, arg_N6, arg_N7 )
    {}
//    {std::cout << "DRV ctor 9"<<std::endl;}

  explicit KOKKOS_INLINE_FUNCTION
  DynRankView( pointer_type arg_ptr
      , typename traits::array_layout & arg_layout
      )
    : DynRankView( Impl::ViewCtorProp<pointer_type>(arg_ptr) , arg_layout )
    {}
//    {std::cout << "DRV ctor 10"<<std::endl;}


  //----------------------------------------
  // Shared scratch memory constructor

  using view_type::shmem_size; 

  explicit KOKKOS_INLINE_FUNCTION
  DynRankView( const typename traits::execution_space::scratch_memory_space & arg_space
      , const size_t arg_N0 = 0 //size_t to ptrdiff_t, default to -1
      , const size_t arg_N1 = 0
      , const size_t arg_N2 = 0
      , const size_t arg_N3 = 0
      , const size_t arg_N4 = 0
      , const size_t arg_N5 = 0
      , const size_t arg_N6 = 0
      , const size_t arg_N7 = 0 )
    : view_type( arg_space
                 , arg_N0 != 0 ? arg_N0 : 1 //if T > 0 ? T : 1
                 , arg_N1 != 0 ? arg_N1 : 1 
                 , arg_N2 != 0 ? arg_N2 : 1 
                 , arg_N3 != 0 ? arg_N3 : 1 
                 , arg_N4 != 0 ? arg_N4 : 1 
                 , arg_N5 != 0 ? arg_N5 : 1 
                 , arg_N6 != 0 ? arg_N6 : 1 
                 , arg_N7 != 0 ? arg_N7 : 1 
               )
    , m_rank( //search for first argN < 0 to terminate / determine rank 
             ( arg_N7 == 0 && arg_N6 == 0 && arg_N5 == 0 && arg_N4 == 0 && arg_N3 == 0 && arg_N2 == 0 && arg_N1 == 0 && arg_N0 == 0) ? 0 
             : ( (arg_N7 == 0 && arg_N6 == 0 && arg_N5 == 0 && arg_N4 == 0 && arg_N3 == 0 && arg_N2 == 0 && arg_N1 == 0) ? 1 
             : ( (arg_N7 == 0 && arg_N6 == 0 && arg_N5 == 0 && arg_N4 == 0 && arg_N3 == 0 && arg_N2 == 0) ? 2 
             : ( (arg_N7 == 0 && arg_N6 == 0 && arg_N5 == 0 && arg_N4 == 0 && arg_N3 == 0) ? 3 
             : ( (arg_N7 == 0 && arg_N6 == 0 && arg_N5 == 0 && arg_N4 == 0) ? 4 
             : ( (arg_N7 == 0 && arg_N6 == 0 && arg_N5 == 0) ? 5 
             : ( (arg_N7 == 0 && arg_N6 == 0) ? 6 
             : ( arg_N7 == 0 ? 7 
             : 8 ) ) ) ) ) ) ) 
            ) 
    {}
//    {std::cout << "DRV ctor 11"<<std::endl;}

};

  //----------------------------------------
  //using Subview...
  //original from View

// Carter's
// EXTENT_ONE_t removed
/*
template< typename T >
typename std::conditional< std::is_integral<T>::value
                         , Kokkos::Experimental::Impl::EXTENT_ONE_t
                         , const T & >::type
KOKKOS_INLINE_FUNCTION
variadic_sview_expansion( const T & i ) 
{ 
  return
  typename std::conditional< std::is_integral<T>::value
                         , Kokkos::Experimental::Impl::EXTENT_ONE_t
                         , const T & >::type( i );
}
*/



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Subview mapping.
// Deduce destination view type from source view traits and subview arguments

namespace Impl {

struct DynRankSubviewTag {};

template< class SrcTraits , class ... Args >
struct ViewMapping
  < typename std::enable_if<(
      std::is_same< typename SrcTraits::specialize , void >::value
      &&
      (
        std::is_same< typename SrcTraits::array_layout
                    , Kokkos::LayoutLeft >::value ||
        std::is_same< typename SrcTraits::array_layout
                    , Kokkos::LayoutRight >::value ||
        std::is_same< typename SrcTraits::array_layout
                    , Kokkos::LayoutStride >::value
      ) 
    ), DynRankSubviewTag >::type
  , SrcTraits
  , Args ... >
{
private:

//  static_assert( SrcTraits::rank == sizeof...(Args) ,
//    "Subview mapping requires one argument for each dimension of source View" );

  enum
    { RZ = false
    , R0 = bool(is_integral_extent<0,Args...>::value)
    , R1 = bool(is_integral_extent<1,Args...>::value)
    , R2 = bool(is_integral_extent<2,Args...>::value)
    , R3 = bool(is_integral_extent<3,Args...>::value)
    , R4 = bool(is_integral_extent<4,Args...>::value)
    , R5 = bool(is_integral_extent<5,Args...>::value)
    , R6 = bool(is_integral_extent<6,Args...>::value)
    , R7 = bool(is_integral_extent<7,Args...>::value)
    };
  //Query additional info

  enum { rank = unsigned(R0) + unsigned(R1) + unsigned(R2) + unsigned(R3)
              + unsigned(R4) + unsigned(R5) + unsigned(R6) + unsigned(R7) }; //dyn rank, with correct use

  // Subview's layout
  typedef Kokkos::LayoutStride array_layout ;

  typedef typename SrcTraits::value_type  value_type ;

  typedef value_type******** data_type ;

public:

  typedef Kokkos::Experimental::ViewTraits
    < data_type
    , array_layout 
    , typename SrcTraits::device_type
    , typename SrcTraits::memory_traits > traits_type ;

  typedef Kokkos::Experimental::View
    < data_type
    , array_layout 
    , typename SrcTraits::device_type
    , typename SrcTraits::memory_traits > type ;

  template< class MemoryTraits >
  struct apply {

    static_assert( Kokkos::Impl::is_memory_traits< MemoryTraits >::value , "" );

    typedef Kokkos::Experimental::ViewTraits
      < data_type 
      , array_layout
      , typename SrcTraits::device_type
      , MemoryTraits > traits_type ;

    typedef Kokkos::Experimental::View
      < data_type 
      , array_layout
      , typename SrcTraits::device_type
      , MemoryTraits > type ;
  };


  typedef typename SrcTraits::dimension dimension ;

  template < class Arg0 = int, class Arg1 = int, class Arg2 = int, class Arg3 = int, class Arg4 = int, class Arg5 = int, class Arg6 = int, class Arg7 = int >
  struct ExtentGenerator {
    static SubviewExtents< 8 , rank > generator ( const dimension & dim , Arg0 arg0 = Arg0(), Arg1 arg1 = Arg1(), Arg2 arg2 = Arg2(), Arg3 arg3 = Arg3(), Arg4 arg4 = Arg4(), Arg5 arg5 = Arg5(), Arg6 arg6 = Arg6(), Arg7 arg7 = Arg7() )
    {
       return SubviewExtents< 8 , rank>( dim , arg0 , arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 );
    }
  };


  typedef DynRankView< value_type , array_layout , typename SrcTraits::device_type , typename SrcTraits::memory_traits >  ret_type;

  template < typename T , class ... P >
  KOKKOS_INLINE_FUNCTION
  static ret_type subview( Kokkos::Experimental::View< T******** , P...> const & src 
                    , Args ... args )
    {
//      static_assert(
//        ViewMapping< DstTraits , traits_type , void >::is_assignable ,
//        "Subview destination type must be compatible with subview derived type" );

      typedef ViewMapping< traits_type, void >  DstType ;

     typedef typename std::conditional< (rank==0) , ViewDimension<>
                                                  , typename std::conditional< (rank==1) , ViewDimension<0>
                                                  , typename std::conditional< (rank==2) , ViewDimension<0,0>
                                                  , typename std::conditional< (rank==3) , ViewDimension<0,0,0>
                                                  , typename std::conditional< (rank==4) , ViewDimension<0,0,0,0>
                                                  , typename std::conditional< (rank==5) , ViewDimension<0,0,0,0,0>
                                                  , typename std::conditional< (rank==6) , ViewDimension<0,0,0,0,0,0>
                                                  , typename std::conditional< (rank==7) , ViewDimension<0,0,0,0,0,0,0>
                                                                                         , ViewDimension<0,0,0,0,0,0,0,0>
                                                  >::type >::type >::type >::type >::type >::type >::type >::type  DstDimType ;

      typedef ViewOffset< DstDimType , Kokkos::LayoutStride > dst_offset_type ;
//      typedef typename DstType::offset_type  dst_offset_type ;
      typedef typename DstType::handle_type  dst_handle_type ;

      Kokkos::Experimental::View< T******** , typename traits_type::array_layout , typename traits_type::device_type , typename traits_type::memory_traits > dst( src ) ;

      const SubviewExtents< 8 , rank > extents = 
        ExtentGenerator< Args ... >::generator( src.m_map.m_offset.m_dim , args... ); 

//      const SubviewExtents< 8 , rank >
//        extents( src.m_offset.m_dim , args... );

      dst_offset_type tempdst( src.m_map.m_offset , extents );

      dst.m_map.m_offset.m_dim.N0 = tempdst.m_dim.N0 ;
      dst.m_map.m_offset.m_dim.N1 = tempdst.m_dim.N1 ;
      dst.m_map.m_offset.m_dim.N2 = tempdst.m_dim.N2 ;
      dst.m_map.m_offset.m_dim.N3 = tempdst.m_dim.N3 ;
      dst.m_map.m_offset.m_dim.N4 = tempdst.m_dim.N4 ;
      dst.m_map.m_offset.m_dim.N5 = tempdst.m_dim.N5 ;
      dst.m_map.m_offset.m_dim.N6 = tempdst.m_dim.N6 ;
      dst.m_map.m_offset.m_dim.N7 = tempdst.m_dim.N7 ;

      dst.m_map.m_offset.m_stride.S0 = tempdst.m_stride.S0 ;
      dst.m_map.m_offset.m_stride.S1 = tempdst.m_stride.S1 ;
      dst.m_map.m_offset.m_stride.S2 = tempdst.m_stride.S2 ;
      dst.m_map.m_offset.m_stride.S3 = tempdst.m_stride.S3 ;
      dst.m_map.m_offset.m_stride.S4 = tempdst.m_stride.S4 ;
      dst.m_map.m_offset.m_stride.S5 = tempdst.m_stride.S5 ;
      dst.m_map.m_offset.m_stride.S6 = tempdst.m_stride.S6 ;
      dst.m_map.m_offset.m_stride.S7 = tempdst.m_stride.S7 ;

      dst.m_map.m_handle = dst_handle_type( src.m_map.m_handle +
                                      src.m_map.m_offset( extents.domain_offset(0)
                                                  , extents.domain_offset(1)
                                                  , extents.domain_offset(2)
                                                  , extents.domain_offset(3)
                                                  , extents.domain_offset(4)
                                                  , extents.domain_offset(5)
                                                  , extents.domain_offset(6)
                                                  , extents.domain_offset(7)
                                                  ) );
      return ret_type( dst , rank );
    }
};
} //close Impl



//Possible direction:
// Remove EXTENT_ONE_t
// Cleanup ViewMapping changes ...
template < class Traits , class Arg0 = int, class Arg1 = int, class Arg2 = int, class Arg3 = int, class Arg4 = int, class Arg5 = int, class Arg6 = int, class Arg7 = int >
struct Subdynrankview {

  typedef typename Kokkos::Experimental::Impl::ViewMapping
    < void
    , Traits
    , typename std::conditional<std::is_integral<Arg0>::value, Kokkos::pair<Arg0,Arg0> , Arg0>::type 
    , typename std::conditional<std::is_integral<Arg1>::value, Kokkos::pair<Arg1,Arg1> , Arg1>::type 
    , typename std::conditional<std::is_integral<Arg2>::value, Kokkos::pair<Arg2,Arg2> , Arg2>::type 
    , typename std::conditional<std::is_integral<Arg3>::value, Kokkos::pair<Arg3,Arg3> , Arg3>::type 
    , typename std::conditional<std::is_integral<Arg4>::value, Kokkos::pair<Arg4,Arg4> , Arg4>::type 
    , typename std::conditional<std::is_integral<Arg5>::value, Kokkos::pair<Arg5,Arg5> , Arg5>::type 
    , typename std::conditional<std::is_integral<Arg6>::value, Kokkos::pair<Arg6,Arg6> , Arg6>::type 
    , typename std::conditional<std::is_integral<Arg7>::value, Kokkos::pair<Arg7,Arg7> , Arg7>::type 
    >::type ViewType ;

  //typedef typename Kokkos::Experimental::Impl::ViewMapping < void , Traits , Args... >::type ViewType ;
//  typedef typename Kokkos::Experimental::Impl::ViewMapping < void , Traits , Arg0 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 , Arg6 , Arg7 >::type ViewType ;

  typedef DynRankView< typename ViewType::value_type , typename ViewType::array_layout , typename ViewType::device_type , typename ViewType::memory_traits >  type;

//Pass whole collection of args to a function with a rank number, 
// scan through list for non-integer entry greater than or equal to rank
// pass through; if no more remaining non-integer return the pair
// Would be nice to have something like a stack to add args to, pop them off until non-integer reached

  template< typename T >
  static typename std::enable_if< std::is_integral<T>::value ,Kokkos::pair<T,T> >::type
  KOKKOS_INLINE_FUNCTION
  convert ( const T & i ) 
  { 
    return
    Kokkos::pair<T,T>( i , i+1 );
  }
  
  template< typename T >
  static typename std::enable_if< !std::is_integral<T>::value , const T & >::type
  KOKKOS_INLINE_FUNCTION
  convert ( const T & i ) 
  { 
    return i;
  }
  

  template < class T , class ... P >
  static type subview( const Kokkos::Experimental::View<T******** , P... > & InputView , Arg0 arg0 = Arg0(), Arg1 arg1 = Arg1(), Arg2 arg2 = Arg2(), Arg3 arg3 = Arg3(), Arg4 arg4 = Arg4(), Arg5 arg5 = Arg5(), Arg6 arg6 = Arg6(), Arg7 arg7 = Arg7() )
  {
    const unsigned numArgs = unsigned(!std::is_integral<Arg0>::value) 
                           + unsigned(!std::is_integral<Arg1>::value) 
                           + unsigned(!std::is_integral<Arg2>::value) 
                           + unsigned(!std::is_integral<Arg3>::value) 
                           + unsigned(!std::is_integral<Arg4>::value) 
                           + unsigned(!std::is_integral<Arg5>::value) 
                           + unsigned(!std::is_integral<Arg6>::value) 
                           + unsigned(!std::is_integral<Arg7>::value) ;

    return type( Kokkos::Experimental::subview( InputView , convert(arg0) , convert(arg1) , convert(arg2) , convert(arg3) , convert(arg4) , convert(arg5) , convert(arg6) , convert(arg7) ) , numArgs );
  }

};


// Carter's idea
template< class D , class ... P , class ...Args >
KOKKOS_INLINE_FUNCTION
typename Subdynrankview< Kokkos::Experimental::ViewTraits< D********, P... > , Args... >::type
subdynrankview( const DynRankView< D , P... > &src , Args...args)
{
  if ( src.rank() != sizeof...(Args) )
    { Kokkos::Impl::throw_runtime_exception("Rank is not compatible"); }

  typedef Impl::ViewMapping< Impl::DynRankSubviewTag , Kokkos::Experimental::ViewTraits< D********, P... > , Args... > metafcn ;
  return metafcn::subview( src.ConstDownCast() , args... );

}

// ************************************************** //

} // namespace Experimental
} // namespace Kokkos


namespace Kokkos {
namespace Experimental {

// overload == and !=
template< class LT , class ... LP , class RT , class ... RP >
KOKKOS_INLINE_FUNCTION
bool operator == ( const DynRankView<LT,LP...> & lhs ,
                   const DynRankView<RT,RP...> & rhs )
{
  // Same data, layout, dimensions
  typedef ViewTraits<LT,LP...>  lhs_traits ;
  typedef ViewTraits<RT,RP...>  rhs_traits ;

  return
    std::is_same< typename lhs_traits::const_value_type ,
                  typename rhs_traits::const_value_type >::value &&
    std::is_same< typename lhs_traits::array_layout ,
                  typename rhs_traits::array_layout >::value &&
    std::is_same< typename lhs_traits::memory_space ,
                  typename rhs_traits::memory_space >::value &&
    lhs.rank()       ==  rhs.rank() &&
    lhs.data()       == rhs.data() &&
    lhs.span()       == rhs.span() &&
    lhs.dimension(0) == rhs.dimension(0) &&
    lhs.dimension(1) == rhs.dimension(1) &&
    lhs.dimension(2) == rhs.dimension(2) &&
    lhs.dimension(3) == rhs.dimension(3) &&
    lhs.dimension(4) == rhs.dimension(4) &&
    lhs.dimension(5) == rhs.dimension(5) &&
    lhs.dimension(6) == rhs.dimension(6) &&
    lhs.dimension(7) == rhs.dimension(7);
}

template< class LT , class ... LP , class RT , class ... RP >
KOKKOS_INLINE_FUNCTION
bool operator != ( const DynRankView<LT,LP...> & lhs ,
                   const DynRankView<RT,RP...> & rhs )
{
  return ! ( operator==(lhs,rhs) );
}

} //end Experimental
} //end Kokkos

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

namespace Kokkos {
namespace Experimental {

/** \brief  Deep copy a value from Host memory into a view.  */
template< class DT , class ... DP >
inline
void deep_copy
  ( const DynRankView<DT,DP...> & dst
  , typename ViewTraits<DT,DP...>::const_value_type & value
  )
{
  deep_copy( dst.ConstDownCast() , value );
}

/** \brief  Deep copy into a value in Host memory from a view.  */
template< class ST , class ... SP >
inline
void deep_copy
  ( typename ViewTraits<ST,SP...>::non_const_value_type & dst
  , const DynRankView<ST,SP...> & src
  )
{
  deep_copy( dst , src.ConstDownCast() );
}


//----------------------------------------------------------------------------
/** \brief  A deep copy between views of compatible type */
template< class DT , class ... DP , class ST , class ... SP >
inline
void deep_copy
  ( const DynRankView<DT,DP...> & dst
  , const DynRankView<ST,SP...> & src
  )
{
  deep_copy( dst.ConstDownCast() , src.ConstDownCast() );
}

} //end Experimental
} //end Kokkos


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

namespace Kokkos {
namespace Experimental {

template< class T , class ... P >
inline
typename DynRankView<T,P...>::HostMirror
create_mirror( const DynRankView<T,P...> & src
             , typename std::enable_if<
                 ! std::is_same< typename Kokkos::Experimental::ViewTraits<T,P...>::array_layout
                               , Kokkos::LayoutStride >::value
               >::type * = 0
             )
{
  typedef DynRankView<T,P...>                   src_type ;
  typedef typename src_type::HostMirror  dst_type ;

  return dst_type( std::string( src.label() ).append("_mirror")
                 , src.dimension(0)
                 , src.dimension(1)
                 , src.dimension(2)
                 , src.dimension(3)
                 , src.dimension(4)
                 , src.dimension(5)
                 , src.dimension(6)
                 , src.dimension(7) );
}


template< class T , class ... P >
inline
typename DynRankView<T,P...>::HostMirror
create_mirror( const DynRankView<T,P...> & src
             , typename std::enable_if<
                 std::is_same< typename Kokkos::Experimental::ViewTraits<T,P...>::array_layout
                             , Kokkos::LayoutStride >::value
               >::type * = 0
             )
{
  typedef DynRankView<T,P...>                   src_type ;
  typedef typename src_type::HostMirror  dst_type ;

  Kokkos::LayoutStride layout ;

  layout.dimension[0] = src.dimension(0);
  layout.dimension[1] = src.dimension(1);
  layout.dimension[2] = src.dimension(2);
  layout.dimension[3] = src.dimension(3);
  layout.dimension[4] = src.dimension(4);
  layout.dimension[5] = src.dimension(5);
  layout.dimension[6] = src.dimension(6);
  layout.dimension[7] = src.dimension(7);

  layout.stride[0] = src.stride(0);
  layout.stride[1] = src.stride(1);
  layout.stride[2] = src.stride(2);
  layout.stride[3] = src.stride(3);
  layout.stride[4] = src.stride(4);
  layout.stride[5] = src.stride(5);
  layout.stride[6] = src.stride(6);
  layout.stride[7] = src.stride(7);

  return dst_type( std::string( src.label() ).append("_mirror") , layout );
}

template< class T , class ... P >
inline
typename DynRankView<T,P...>::HostMirror
create_mirror_view( const DynRankView<T,P...> & src
                  , typename std::enable_if<(
                      std::is_same< typename DynRankView<T,P...>::memory_space
                                  , typename DynRankView<T,P...>::HostMirror::memory_space
                                  >::value
                      &&
                      std::is_same< typename DynRankView<T,P...>::data_type
                                  , typename DynRankView<T,P...>::HostMirror::data_type
                                  >::value
                    )>::type * = 0
                  )
{
  return src ;
}

template< class T , class ... P >
inline
typename DynRankView<T,P...>::HostMirror
create_mirror_view( const DynRankView<T,P...> & src
                  , typename std::enable_if< ! (
                      std::is_same< typename DynRankView<T,P...>::memory_space
                                  , typename DynRankView<T,P...>::HostMirror::memory_space
                                  >::value
                      &&
                      std::is_same< typename DynRankView<T,P...>::data_type
                                  , typename DynRankView<T,P...>::HostMirror::data_type
                                  >::value
                    )>::type * = 0
                  )
{
  return Kokkos::Experimental::create_mirror( src ); 
}

} //end Experimental
} //end Kokkos


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

namespace Kokkos {
namespace Experimental {

/** \brief  Resize a view with copying old data to new data at the corresponding indices. */
template< class T , class ... P >
inline
void resize( DynRankView<T,P...> & v ,
             const size_t n0 = 0 ,
             const size_t n1 = 0 ,
             const size_t n2 = 0 ,
             const size_t n3 = 0 ,
             const size_t n4 = 0 ,
             const size_t n5 = 0 ,
             const size_t n6 = 0 ,
             const size_t n7 = 0 )
{
  typedef DynRankView<T,P...>  drview_type ;

  static_assert( Kokkos::Experimental::ViewTraits<T,P...>::is_managed , "Can only resize managed views" );

  drview_type v_resized( v.label(), n0, n1, n2, n3, n4, n5, n6, n7 );

  Kokkos::Experimental::Impl::ViewRemap< drview_type , drview_type >( v_resized , v );

  v = v_resized ;
}

/** \brief  Resize a view with copying old data to new data at the corresponding indices. */
template< class T , class ... P >
inline
void realloc( DynRankView<T,P...> & v ,
              const size_t n0 = 0 ,
              const size_t n1 = 0 ,
              const size_t n2 = 0 ,
              const size_t n3 = 0 ,
              const size_t n4 = 0 ,
              const size_t n5 = 0 ,
              const size_t n6 = 0 ,
              const size_t n7 = 0 )
{
  typedef DynRankView<T,P...>  view_type ;

  static_assert( Kokkos::Experimental::ViewTraits<T,P...>::is_managed , "Can only realloc managed views" );

  const std::string label = v.label();

  v = view_type(); // Deallocate first, if the only view to allocation
  v = view_type( label, n0, n1, n2, n3, n4, n5, n6, n7 );
}

} //end Experimental
} //end Kokkos

#endif
