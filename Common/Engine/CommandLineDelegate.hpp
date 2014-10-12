#pragma once
#ifndef INCLUDED_COMMAND_LINE_DELEGATE_HPP
#define INCLUDED_COMMAND_LINE_DELEGATE_HPP

//-----------------------------------------------------------------------------------------------
#include "CommandLineOption.hpp"

//-----------------------------------------------------------------------------------------------
namespace CommandLine
{
	typedef std::vector< CommandLine::Option > OptionList;


	//-------------------------------------------------------------------------------------------
	class Delegate
	{
		typedef void* ObjectInstancePointer;
		typedef void ( *WrappedFunction )( ObjectInstancePointer, OptionList );

		struct DelegatedFunction
		{
			DelegatedFunction( ObjectInstancePointer objectInstance, WrappedFunction wrappedFunction )
				: objectInstance( objectInstance )
				, wrappedFunction( wrappedFunction )
			{ }

			//Data Members
			ObjectInstancePointer objectInstance;
			WrappedFunction wrappedFunction;
		};



	public:
		Delegate()
			: m_delegatedFunction( nullptr, nullptr )
		{ }

		//Public Interface
		template < void ( *StaticFunction )( OptionList ) >
		void Bind();

		template < typename ObjectType, void ( ObjectType::*MemberFunction )( OptionList ) >
		void Bind( ObjectType* objectInstance );

		void Invoke( OptionList options ) const;

		bool operator==( const CommandLine::Delegate& other ) const;

	private:
		//Function Wrapping
		template < void ( *StaticFunction )( OptionList ) >
		static __forceinline void WrapStaticFunction( ObjectInstancePointer, OptionList options );

		template < typename ObjectType, void ( ObjectType::*MemberFunction )( OptionList ) >
		static __forceinline void WrapMemberFunction( ObjectInstancePointer objectInstance, OptionList options );

		//Data Member
		DelegatedFunction m_delegatedFunction;
	};



	#pragma region Public Interface
	//-------------------------------------------------------------------------------------------
	template < void ( *StaticFunction )( OptionList ) >
	inline void Delegate::Bind()
	{
		m_delegatedFunction.objectInstance = nullptr;
		m_delegatedFunction.wrappedFunction = &WrapStaticFunction< StaticFunction >;
	}

	//-------------------------------------------------------------------------------------------
	template < typename ObjectType, void ( ObjectType::*MemberFunction )( OptionList ) >
	inline void Delegate::Bind( ObjectType* objectInstance )
	{
		m_delegatedFunction.objectInstance = objectInstance;
		m_delegatedFunction.wrappedFunction = &WrapMemberFunction< MemberFunction >;
	}

	//-------------------------------------------------------------------------------------------
	inline void Delegate::Invoke( OptionList options ) const
	{
		FATAL_ASSERTION( m_delegatedFunction.wrappedFunction != nullptr, "Delegation Error",
						"Cannot invoke unbound command line delegate. Call Bind() first.\n" );

		return m_delegatedFunction.wrappedFunction( m_delegatedFunction.objectInstance, options );
	}

	//-------------------------------------------------------------------------------------------
	inline bool Delegate::operator==( const CommandLine::Delegate& other ) const
	{
		if( m_delegatedFunction.objectInstance == other.m_delegatedFunction.objectInstance &&
			m_delegatedFunction.wrappedFunction == other.m_delegatedFunction.wrappedFunction )
		{
			return true;
		}
		return false;
	}
	#pragma endregion



	#pragma region Function Wrapping
	template < void ( *StaticFunction )( OptionList ) >
	static __forceinline void Delegate::WrapStaticFunction( ObjectInstancePointer, OptionList options )
	{
		return ( StaticFunction )( options );
	}

	template < typename ObjectType, void ( ObjectType::*MemberFunction )( OptionList ) >
	static __forceinline void Delegate::WrapMemberFunction( ObjectInstancePointer objectInstance, OptionList options )
	{
		return ( static_cast< ObjectType* >( objectInstance )->*MemberFunction )( options );
	}
	#pragma endregion
}
#endif //INCLUDED_COMMAND_LINE_DELEGATE_HPP
