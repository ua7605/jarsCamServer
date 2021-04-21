package be.uantwerpen.idlab.dust.core;

import java.io.IOException;
import java.nio.file.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Consumer;

public class Core {

    private final long nativePointer;
    private Path configurationPath;

    private Thread cycleThread;
    private final AtomicBoolean cycleRunning = new AtomicBoolean(false);

    public Core(String name) {
        nativePointer = Native.constructor(name);
    }

    public Core(String name, String modulesDirectory) {
        nativePointer = Native.constructor(name, modulesDirectory);
    }

    public void publish(String name, byte[] payload) {
        Native.publish(nativePointer, name, payload);
    }

    public boolean registerListener(String channel, Consumer<byte[]> callback) {
        return Native.registerListener(nativePointer, channel, new CallbackWrapper(callback));
    }

    public String[] getChannels() {
        return Native.getChannels(nativePointer);
    }

    public void setup() {
        Native.setup(nativePointer);
    }

    public void connect() {
        Native.connect(nativePointer);
    }

    public void disconnect() {
        Native.disconnect(nativePointer);
    }

    public void setConfigurationFile(String path) {
        configurationPath = Paths.get(path);
        Native.setConfigurationFile(nativePointer, path);
    }

    public void reloadConfigurationFile() {
        Native.reloadConfigurationFile(nativePointer);
    }

    public void parseConfigurationFile(String path) {
        configurationPath = Paths.get(path);
        Native.parseConfigurationFile(nativePointer, path);
    }

    public String getName() {
        return Native.getName(nativePointer);
    }

    public void cycleOnce() {
        Native.cycleOnce(nativePointer);
    }

    public void waitForChange() {
        Native.waitForChange(nativePointer);
    }

    public Thread cycleForever() {
        if (cycleRunning.get() && cycleThread != null) {
            return cycleThread;
        }
        cycleRunning.set(true);
        cycleThread = new Thread(() -> {
            while (cycleRunning.get()) {
                waitForChange();
                cycleOnce();
            }
        });
        cycleThread.start();
        return cycleThread;
    }

    public void cycleStop() {
        cycleRunning.set(false);
    }

    public void watchConfigurationFile() {
        try (WatchService watcher = FileSystems.getDefault().newWatchService()) {
            Path path = configurationPath.getParent();
            path.register(watcher, StandardWatchEventKinds.ENTRY_MODIFY);

            Thread t = new Thread(() -> {
                while (cycleRunning.get()) {
                    WatchKey key;
                    try {
                        key = watcher.poll(25, TimeUnit.MILLISECONDS);
                    } catch (InterruptedException e) {
                        return;
                    }
                    if (key == null) {
                        Thread.yield();
                        continue;
                    }

                    for (WatchEvent<?> event : key.pollEvents()) {
                        WatchEvent.Kind<?> kind = event.kind();

                        @SuppressWarnings("unchecked")
                        WatchEvent<Path> ev = (WatchEvent<Path>) event;
                        Path filename = ev.context();

                        if (kind == StandardWatchEventKinds.OVERFLOW) {
                            continue;
                        } else if (kind == StandardWatchEventKinds.ENTRY_MODIFY && filename.toString().equals(path.getFileName().toString())) {

                            reloadConfigurationFile();
                        }
                        boolean valid = key.reset();
                        if (!valid) {
                            break;
                        }
                    }
                }
            });
            t.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static class CallbackWrapper implements Consumer<byte[]> {

        private final ExecutorService executor = Executors.newSingleThreadExecutor(
                r -> {
                    Thread t = Executors.defaultThreadFactory().newThread(r);
                    t.setDaemon(true);
                    return t;
                });

        private final Consumer<byte[]> callbackFunction;

        private CallbackWrapper(Consumer<byte[]> callbackFunction) {
            this.callbackFunction = callbackFunction;
        }

        @Override
        public void accept(byte[] bytes) {
            executor.submit(() -> callbackFunction.accept(bytes));
        }

    }

    private static class Native {

        static {
            Loader.load();
        }

        private static native long constructor(String name);

        private static native long constructor(String name, String modulesDirectory);

        private static native void destructor(long pointer);

        private static native void publish(long pointer, String name, byte[] payload);

        private static native boolean registerListener(long pointer, String channel, CallbackWrapper callback);

        private static native String[] getChannels(long pointer);

        private static native void setup(long pointer);

        private static native void connect(long pointer);

        private static native void disconnect(long pointer);

        private static native void setConfigurationFile(long pointer, String path);

        private static native void reloadConfigurationFile(long pointer);

        private static native void parseConfigurationFile(long pointer, String path);

        private static native String getName(long pointer);

        private static native void cycleOnce(long pointer);

        private static native void waitForChange(long pointer);

    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        Native.destructor(nativePointer);
    }

}
