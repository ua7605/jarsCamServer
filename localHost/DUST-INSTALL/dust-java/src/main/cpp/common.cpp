#include "common.h"

std::string jstringToStdString(JNIEnv *env, jstring jStr)
{
    if (!jStr)
    {
        return "";
    }

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes =
        env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const auto stringJbytes = (jbyteArray)env->CallObjectMethod(
        jStr, getBytes, env->NewStringUTF("UTF-8"));

    auto length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, nullptr);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

jstring stdStringToJstring(JNIEnv *env, std::string string)
{
    return env->NewStringUTF(string.c_str());
}

jint throwException(JNIEnv *env, char *className, std::string message)
{
    jclass exClass;

    exClass = env->FindClass(className);
    if (exClass == NULL)
    {
        return throwException(env, "java/lang/NoClassDefFoundError", "test");
    }

    return env->ThrowNew(exClass, message.data());
}