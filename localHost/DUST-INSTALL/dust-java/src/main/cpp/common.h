#ifndef _DUST_JAVA_COMMON
#define _DUST_JAVA_COMMON

#include <jni.h>
#include <string>

std::string jstringToStdString(JNIEnv *env, jstring jStr);

jstring stdStringToJstring(JNIEnv *env, std::string string);

template <class T>
T *jlongToClass(jlong pointer)
{
    return reinterpret_cast<T *>(pointer);
}

jint throwException(JNIEnv *env, char *className, std::string message);

#endif