
cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_program(VALGRIND "valgrind")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(raytracer main.cpp)
add_executable(test_runner tests/test_runner.cpp)

target_include_directories(raytracer PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/src/raymath"
        "${PROJECT_SOURCE_DIR}/src/rayimage"
        "${PROJECT_SOURCE_DIR}/src/rayscene"
)

target_include_directories(test_runner PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/src/raymath"
        "${PROJECT_SOURCE_DIR}/src/rayimage"
        "${PROJECT_SOURCE_DIR}/src/rayscene"
)

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/rayscene)
add_subdirectory(./src/lodepng)

target_link_libraries(raytracer
        PUBLIC
        rayscene
        raymath
        rayimage
        lodepng
        Threads::Threads
)

# Linking pour test_runner
target_link_libraries(test_runner
        PUBLIC
        rayscene
        raymath
        rayimage
        lodepng
        Threads::Threads
)

find_package(OpenSSL REQUIRED)
target_link_libraries(test_runner PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Liez Eigen aux deux executables

enable_testing()

# Test basique - vérifie si le programme s'exécute
add_test(NAME basic_execution
        COMMAND raytracer ../scenes/two-spheres-on-plane.json)

# === Tests cas limites (3 points) ===
# Test avec un fichier malformaté
add_test(NAME edge_case_malformated_file
        COMMAND raytracer ../scenes/malformated.json)
set_tests_properties(edge_case_malformated_file
        PROPERTIES WILL_FAIL FALSE)
# Test avec un fichier vide
add_test(NAME edge_case_empty_file
        COMMAND raytracer ../scenes/empty.json)
set_tests_properties(edge_case_empty_file
        PROPERTIES WILL_FAIL FALSE)

# === Démonstration d'échec (4 points) ===
# Test avec un fichier qui n'existe pas
add_test(NAME edge_case_nonexistent_file
        COMMAND raytracer nonexistent.json)
set_tests_properties(edge_case_nonexistent_file
        PROPERTIES WILL_FAIL TRUE)
# Test de sortie d'erreur - sans arguments
add_test(NAME failure_no_args
        COMMAND raytracer)
set_tests_properties(failure_no_args
        PROPERTIES
        WILL_FAIL FALSE
        FAIL_REGULAR_EXPRESSION "[ERROR] Please a path your scene file (.json)")

# === Tests fonctionnel (10 points)
# Test avec vérification du fichier de sortie
#add_test(NAME output_file_test
#        COMMAND raytracer ../scenes/two-spheres-on-plane.json output.png)
#set_tests_properties(output_file_test
#        PROPERTIES PASS_REGULAR_EXPRESSION "Writing file: output.png")
#
## Test de performance
#add_test(NAME performance_check
#        COMMAND raytracer ../scenes/two-spheres-on-plane.json perf_output.png)
#set_tests_properties(performance_check
#        PROPERTIES PASS_REGULAR_EXPRESSION "Total time: .*seconds")
#
## Test de résolution d'image
#add_test(NAME resolution_check
#        COMMAND raytracer ../scenes/two-spheres-on-plane.json res_output.png)
#set_tests_properties(resolution_check
#        PROPERTIES PASS_REGULAR_EXPRESSION "[0-9]+x[0-9]+ pixels")

# Test sur l'image générée
add_test(NAME image_comparison_test
        COMMAND test_runner
        ${CMAKE_SOURCE_DIR}/scenes/two-spheres-on-plane.json
        ${CMAKE_SOURCE_DIR}/tests/reference/two-spheres-on-plane.png)
add_test(NAME image_comparison_test2
        COMMAND test_runner
        ${CMAKE_SOURCE_DIR}/scenes/two-triangles-on-plane.json
        ${CMAKE_SOURCE_DIR}/tests/reference/two-triangles-on-plane.png)

if (VALGRIND)
    add_test(NAME memory_check
            COMMAND valgrind
            ${CMAKE_BINARY_DIR}/raytracer ../scenes/two-spheres-on-plane.json valgrind_output.png)
endif ()
