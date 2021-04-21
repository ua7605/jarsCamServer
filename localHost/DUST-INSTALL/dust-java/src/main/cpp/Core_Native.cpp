#include "Core_Native.h"
#include "common.h"

JNIEXPORT jlong JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_constructor__Ljava_lang_String_2(JNIEnv *env, jclass staticClass, jstring blockName)
{
    std::string blockNameString = jstringToStdString(env, blockName);
    uint64_t ptr = reinterpret_cast<uint64_t>(new dust::Core(blockNameString));
    return ptr;
}

JNIEXPORT jlong JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_constructor__Ljava_lang_String_2Ljava_lang_String_2(JNIEnv *env, jclass staticClass, jstring blockName, jstring directoryPath)
{
    std::string blockNameString = jstringToStdString(env, blockName);
    std::string directoryPathString = jstringToStdString(env, directoryPath);
    auto ptr = reinterpret_cast<uint64_t>(new dust::Core(blockNameString, directoryPathString));
    return ptr;
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_destructor(JNIEnv *env, jclass staticClass, jlong pointer)
{
    delete jlongToClass<dust::Core>(pointer);
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_publish(JNIEnv *env, jclass staticClass, jlong pointer, jstring channelName, jbyteArray byteArray)
{
    std::vector<uint8_t> payload;
    jsize size = env->GetArrayLength(byteArray);
    payload.reserve((size_t)size);
    for (int i = 0; i < (size_t)size; ++i)
    {
        jbyte *b = (jbyte *)env->GetByteArrayElements(byteArray, nullptr);

        payload.push_back((uint8_t)b[i]);

        env->ReleaseByteArrayElements(byteArray, b, 0);
    }

    try
    {
        jlongToClass<dust::Core>(pointer)->publish(jstringToStdString(env, channelName), payload);
    }
    catch (dust::exceptions::DustException &exception)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/DustException", exception.what());
    }
    catch (...)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/NativeException", "Exception thrown at publish");
    }
}

JNIEXPORT jboolean JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_registerListener(JNIEnv *env, jclass staticClass, jlong pointer, jstring channelName, jobject consumerObject)
{
    dust::Core *core = jlongToClass<dust::Core>(pointer);
    JavaVM *jvm;
    env->GetJavaVM(&jvm);
    jobject globalConsumerObject = env->NewGlobalRef(consumerObject);

    std::function<void(const std::vector<uint8_t> &)> callbackFunction = [jvm, globalConsumerObject](const std::vector<uint8_t> &payload) {
        JNIEnv *env;
        JavaVMAttachArgs args;
        args.version = JNI_VERSION_1_6;
        args.name = nullptr;
        args.group = nullptr;

        // attaches the current thread to JVM and sets the JNIEnv variable.
        jvm->AttachCurrentThread((void **)&env, &args);

        jclass clazz = env->GetObjectClass(globalConsumerObject);
        jmethodID acceptMethod = env->GetMethodID(clazz, "accept", "([B)V");

        jsize payloadSize = (jsize)payload.size();
        jbyteArray bytes = env->NewByteArray(payloadSize);
        env->SetByteArrayRegion(bytes, 0, payloadSize, (jbyte *)payload.data());

        env->CallVoidMethod(globalConsumerObject, acceptMethod, bytes);

        env->DeleteLocalRef((jobject)bytes);
        // env->DeleteLocalRef((jobject)acceptMethod);
        // env->DeleteLocalRef((jobject)clazz);

        jvm->DetachCurrentThread();
    };

    try
    {
        return core->registerListener(jstringToStdString(env, channelName), callbackFunction);
    }
    catch (dust::exceptions::DustException &exception)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/DustException", exception.what());
    }
    catch (...)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/NativeException", "Exception thrown at publish");
    }
}

JNIEXPORT jobjectArray JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_getChannels(JNIEnv *env, jclass staticClass, jlong pointer)
{
    dust::Core *core = jlongToClass<dust::Core>(pointer);
    jobjectArray objectArray = env->NewObjectArray((jsize)core->getChannels().size(), env->FindClass("java/lang/String"), nullptr);
    jsize i = 0;
    for (const auto &channelEntry : core->getChannels())
    {
        env->SetObjectArrayElement(objectArray, i++, (jobject)stdStringToJstring(env, channelEntry.first));
    }
    return objectArray;
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_setup(JNIEnv *env, jclass staticClass, jlong pointer)
{
    try
    {
        jlongToClass<dust::Core>(pointer)->setup();
    }
    catch (dust::exceptions::DustException &exception)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/DustException", exception.what());
    }
    catch (...)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/NativeException", "Exception thrown at publish");
    }
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_connect(JNIEnv *env, jclass staticClass, jlong pointer)
{
    jlongToClass<dust::Core>(pointer)->connect();
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_disconnect(JNIEnv *env, jclass staticClass, jlong pointer)
{
    jlongToClass<dust::Core>(pointer)->disconnect();
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_setConfigurationFile(JNIEnv *env, jclass staticClass, jlong pointer, jstring configurationFilePath)
{
    jlongToClass<dust::Core>(pointer)->setConfigurationFile(jstringToStdString(env, configurationFilePath));
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_reloadConfigurationFile(JNIEnv *env, jclass staticClass, jlong pointer)
{
    jlongToClass<dust::Core>(pointer)->reloadConfigurationFile();
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_parseConfigurationFile(JNIEnv *env, jclass staticClass, jlong pointer, jstring configurationFilePath)
{
    try
    {
        jlongToClass<dust::Core>(pointer)->parseConfigurationFile(jstringToStdString(env, configurationFilePath));
    }
    catch (dust::exceptions::DustException &exception)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/DustException", exception.what());
    }
    catch (...)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/NativeException", "Exception thrown at publish");
    }
}

JNIEXPORT jstring JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_getName(JNIEnv *env, jclass staticClass, jlong pointer)
{
    return stdStringToJstring(env, jlongToClass<dust::Core>(pointer)->getName());
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_cycleOnce(JNIEnv *env, jclass staticClass, jlong pointer)
{
    try
    {
        jlongToClass<dust::Core>(pointer)->cycleOnce();
    }
    catch (dust::exceptions::DustException &exception)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/DustException", exception.what());
    }
    catch (...)
    {
        throwException(env, "be/uantwerpen/idlab/dust/core/exceptions/NativeException", "Exception thrown at publish");
    }
}

JNIEXPORT void JNICALL Java_be_uantwerpen_idlab_dust_core_Core_00024Native_waitForChange(JNIEnv *env, jclass staticClass, jlong pointer)
{
    jlongToClass<dust::Core>(pointer)->waitForChange();
}
