/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	cdecoderProxy

*********************************************************************/

#ifndef  _CONSTRUCTOR_MAGIC_H_
#define  _CONSTRUCTOR_MAGIC_H_

// Put this in the declarations for a class to be unassignable.
#define  _DISALLOW_ASSIGN(TypeName) \
  TypeName& operator=(const TypeName&);

// A macro to disallow the copy constructor and operator= functions. This should
// be used in the declarations for a class.
#define  _DISALLOW_COPY_AND_ASSIGN(TypeName) \
  private:										\
  TypeName(const TypeName&);        \
   _DISALLOW_ASSIGN(TypeName)

// A macro to disallow all the implicit constructors, namely the default
// constructor, copy constructor and operator= functions.
//
// This should be used in the declarations for a class that wants to prevent
// anyone from instantiating it. This is especially useful for classes
// containing only static methods.
#define  _DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                               \
  RTC_DISALLOW_COPY_AND_ASSIGN(TypeName)

#endif  // RTC_BASE_CONSTRUCTOR_MAGIC_H_
