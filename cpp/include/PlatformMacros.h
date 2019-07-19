#pragma once

/**
 * Define some platform specific macros.
 */
#include "PlatformConfig.h"
#include "PlatformDefine.h"


// Generic macros

/// @name namespace cocos2d
/// @{
#ifdef __cplusplus
    #define NS_BEGIN(ns)                     namespace ns {
    #define NS_END                       }
    #define USING_NS(ns)                     using namespace ns
#else
    #define NS_BEGIN(ns) 
    #define NS_END 
    #define USING_NS(ns) 
#endif 
//  end of namespace group
/// @}

/** @def PROPERTY 
 * It is used to declare a protected variable.
 * We can use getter to read the variable, and use the setter to change the variable.
 *
 * @param varType     The type of variable.
 * @param varName     Variable name.
 * @param funName     "get + funName" will be the name of the getter.
 *                    "set + funName" will be the name of the setter.
 * @warning   The getter and setter are public virtual functions, you should rewrite them first.
 *            The variables and methods declared after PROPERTY are all public.
 *            If you need protected or private, please declare.
 */
#define PROPERTY(varType, varName, funName)\
protected: varType varName; public: inline const varType& get##funName(void) const { return varName; } inline void set##funName(const varType& var){ varName = var; }


#define SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define BREAK_IF(cond)           if(cond) break


#define IF_RETURN_VOID(condition) if(condition) return;
#define IF_RETURN(condition, ret) if(condition) return ret;