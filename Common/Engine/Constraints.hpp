#ifndef INCLUDED_CONSTRAINTS_HPP
#define INCLUDED_CONSTRAINTS_HPP

//-----------------------------------------------------------------------------------------------
//These two "classes" are constraints used in my math classes. 
//With these, the compiler will require addition and multiplication on classes that instantiate my math classes at compile time.
//Code adapted from Bjarne Stroustrop: http://www.stroustrup.com/bs_faq2.html#constraints
template<class T1, class T2, class T3 = T1> struct Can_Add 
{
	static void AddConstraint(T1 a, T2 b, T3 c) { c = a + b; }
	static void AddEqualConstraint(T1 a, T2 b) { a += b; }
	Can_Add() 
	{ 
		void(*p)(T1,T2,T3) = AddConstraint;
		void(*p2)(T1,T2) = AddEqualConstraint; 
	}
};

template<class T1, class T2, class T3 = T1> struct Can_Multiply 
{
	static void MultiplyConstraint(T1 a, T2 b, T3 c) { c = a * b; }
	static void MultiplyEqualConstraint(T1 a, T2 b) { a *= b; }
	Can_Multiply() 
	{ 
		void(*p)(T1,T2,T3) = MultiplyConstraint;
		void(*p2)(T1,T2) = MultiplyEqualConstraint; 
	}
};

#endif // INCLUDED_CONSTRAINTS_HPP
