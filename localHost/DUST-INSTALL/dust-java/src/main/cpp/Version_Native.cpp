#include "Version_Native.h"
#include "common.h"

JNIEXPORT jlong JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_constructor__I(JNIEnv *, jclass, jint major)
{
    uint64_t ptr = reinterpret_cast<uint64_t>(new dust::Version((unsigned int)major));
    return ptr;
}

JNIEXPORT jlong JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_constructor__II(JNIEnv *, jclass, jint major, jint minor)
{
    uint64_t ptr = reinterpret_cast<uint64_t>(new dust::Version((unsigned int)major, (unsigned int)minor));
    return ptr;
}

JNIEXPORT jlong JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_constructor__III(JNIEnv *, jclass, jint major, jint minor, jint patch)
{
    uint64_t ptr = reinterpret_cast<uint64_t>(new dust::Version((unsigned int)major, (unsigned int)minor, (unsigned int)patch));
    return ptr;
}

JNIEXPORT jlong JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_getDustVersion(JNIEnv *, jclass)
{
    uint64_t ptr = reinterpret_cast<uint64_t>(&dust::Version::DUST_VERSION);
    return ptr;
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_destructor(JNIEnv *, jclass, jlong pointer)
{
    delete jlongToClass<dust::Version>(pointer);
}

JNIEXPORT jint JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_getMajor(JNIEnv *, jclass, jlong pointer)
{
    return jlongToClass<dust::Version>(pointer)->getMajor().value();
}

JNIEXPORT jboolean JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_hasMajor(JNIEnv *, jclass, jlong pointer)
{
    return false;
}

JNIEXPORT jint JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_getMinor(JNIEnv *, jclass, jlong pointer)
{
    return jlongToClass<dust::Version>(pointer)->getMinor().value();
}

JNIEXPORT jboolean JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_hasMinor(JNIEnv *, jclass, jlong pointer)
{
    return false;
}

JNIEXPORT jint JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_getPatch(JNIEnv *, jclass, jlong pointer)
{
    return jlongToClass<dust::Version>(pointer)->getPatch().value();
}

JNIEXPORT jboolean JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_hasPatch(JNIEnv *, jclass, jlong pointer)
{
    return false;
}

JNIEXPORT jstring JNICALL Java_be_uantwerpen_idlab_dust_core_Version_00024Native_getVersionString(JNIEnv *env, jclass, jlong pointer)
{
    return stdStringToJstring(env, jlongToClass<dust::Version>(pointer)->getVersionString());
}