#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "Image.hpp"
#include <openssl/md5.h> // Si disponible sur votre système
#include <SceneLoader.hpp>

class ImageComparator {
public:
    static std::string getFileHash(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Cannot open file: " << filename << std::endl;
            return "";
        }

        // Lire le fichier
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        // Calculer le hash MD5
        unsigned char result[MD5_DIGEST_LENGTH];
        MD5((unsigned char*)content.c_str(), content.size(), result);

        // Convertir en string hexadécimal
        std::stringstream ss;
        for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)result[i];
        }

        return ss.str();
    }

    static bool compareImages(const std::string& generated, const std::string& reference) {
        std::string hash1 = getFileHash(generated);
        std::string hash2 = getFileHash(reference);

        std::cout << "Generated image hash: " << hash1 << std::endl;
        std::cout << "Reference image hash: " << hash2 << std::endl;

        return hash1 == hash2;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <scene_file> <reference_image>" << std::endl;
        return 1;
    }

    std::string scenePath = argv[1];
    std::string referencePath = argv[2];
    std::string outputPath = referencePath + ".png";

    try {
        auto [scene, camera, image] = SceneLoader::Load(scenePath);
        camera->render(*image, *scene);
        image->writeFile(outputPath);

        if (ImageComparator::compareImages(outputPath, referencePath)) {
            std::cout << "Test passed: Images match" << std::endl;
            return 0;
        } else {
            std::cout << "Test failed: Images don't match" << std::endl;
            return 1;
        }

        delete scene;
        delete camera;
        delete image;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during test: " << e.what() << std::endl;
        return 1;
    }
}