package be.uantwerpen.idlab.dust.core;

import java.io.*;
import java.net.URL;
import java.nio.file.*;
import java.nio.file.attribute.FileTime;
import java.util.Optional;
import java.util.Properties;
import java.util.concurrent.atomic.AtomicBoolean;

public class Loader {

    private final static AtomicBoolean isLoaded = new AtomicBoolean(false);

    public static void load() {
        if (isLoaded.get()) {
            return;
        }

        try {
            String platformSpecificLibraryName = System.mapLibraryName("_core");
            Path path = cacheLibrary(platformSpecificLibraryName);

            System.load(path.toAbsolutePath().toString());
            isLoaded.set(true);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static Path cacheLibrary(String library) throws IOException {
        Optional<Path> cachedLibraryPath = getCachedLibraryPath(library);

        if (!cachedLibraryPath.isPresent()) {
            throw new IllegalArgumentException("shit");
        }

        return cachedLibraryPath.get();
    }

    private static Optional<Path> getCachedLibraryPath(String library) {
        try {
            String version = "unknown";
            try (InputStream dustVersion = Loader.class.getResourceAsStream("/META-INF/maven/be.uantwerpen.idlab.dust/dust-core/pom.properties")) {
                if (dustVersion != null) {
                    Properties properties = new Properties();
                    properties.load(dustVersion);
                    if (properties.containsKey("version")) {
                        version = properties.getProperty("version");
                    }
                }
            }

            Path tempLibraryFile = Paths.get(System.getProperty("java.io.tmpdir"), "dust", "cache", "java", version, library);
            if (version.equals("unknown") && Files.exists(tempLibraryFile)) {
                Files.delete(tempLibraryFile);
            }
            if (!Files.exists(tempLibraryFile) || checkIfArchivedIsNewer(library, tempLibraryFile)) {
                Files.createDirectories(tempLibraryFile.getParent());
                extractLibraryToPath(library, tempLibraryFile);
            }
            return Optional.of(tempLibraryFile);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return Optional.empty();
    }

    private static boolean checkIfArchivedIsNewer(String library, Path cachePath) {
        URL resource = Loader.class.getClassLoader().getResource(library);
        try {
            assert resource != null;
            long lastModifiedArchive = resource.openConnection().getLastModified();
            FileTime lastModifiedCache = Files.getLastModifiedTime(cachePath);
            if (lastModifiedCache.toMillis() < lastModifiedArchive) {
                return true;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }

    private static void extractLibraryToPath(String library, Path path) {
        try (InputStream resourceAsStream = Loader.class.getClassLoader().getResourceAsStream(library)) {
            if (resourceAsStream == null) {
                throw new FileNotFoundException();
            }
            try {
                Files.copy(resourceAsStream, path, StandardCopyOption.REPLACE_EXISTING);
            } catch (AccessDeniedException e) {
                System.err.println("Unable to save native library files to disk");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
