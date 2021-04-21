package be.uantwerpen.idlab.dust.core;

public class Version {

    private final long nativePointer;
    private final boolean preventDestruction;

    private Version(long nativePointer) {
        this.nativePointer = nativePointer;
        preventDestruction = true;
    }

    public Version(int major) {
        nativePointer = Native.constructor(major);
        preventDestruction = false;
    }

    public Version(int major, int minor) {
        nativePointer = Native.constructor(major, minor);
        preventDestruction = false;
    }

    public Version(int major, int minor, int patch) {
        nativePointer = Native.constructor(major, minor, patch);
        preventDestruction = false;
    }

    public int getMajor() {
        return Native.getMajor(nativePointer);
    }

    public int getMinor() {
        return Native.getMinor(nativePointer);
    }

    public int getPatch() {
        return Native.getPatch(nativePointer);
    }

    public static Version getDustVersion() {
        return new Version(Native.getDustVersion());
    }

    public String getVersionString() {
        return Native.getVersionString(nativePointer);
    }

    private static class Native {

        static {
            Loader.load();
        }

        private static native long constructor(int major);

        private static native long constructor(int major, int minor);

        private static native long constructor(int major, int minor, int patch);

        private static native long getDustVersion();

        private static native void destructor(long pointer);

        private static native int getMajor(long pointer);

        private static native boolean hasMajor(long pointer);

        private static native int getMinor(long pointer);

        private static native boolean hasMinor(long pointer);

        private static native int getPatch(long pointer);

        private static native boolean hasPatch(long pointer);

        private static native String getVersionString(long pointer);

    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        if (!preventDestruction) {
            Native.destructor(nativePointer);
        }
    }
}
