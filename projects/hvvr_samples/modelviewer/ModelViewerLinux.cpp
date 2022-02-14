//
// Created by steven on 1/21/22.
//

#include "ModelViewerLinux.h"
#include "camera.h"
#include "samples.h"
#include "vector_math.h"
#include "model_import.h"
#include <cstring>
#include <iostream>

int main() {
    setenv("DISPLAY", ":1", true);
    ModelViewerLinux::Settings settings;
    settings.initScene = gSceneSpecs[3];
    ModelViewerLinux viewer{ settings };
    viewer.onInit();
    viewer.run();
    return 0;
}

ModelViewerLinux::ModelViewerLinux(ModelViewerLinux::Settings settings) {
    m_settings = settings;

    createGLWindow();
}
void ModelViewerLinux::onInit() {
    RayCasterSpecification spec;
    if (m_cameraSettings.foveatedCamera) {
        spec = RayCasterSpecification::feb2017FoveatedDemoSettings();
    }
    spec.outputTo3DApi = (m_settings.outputMode == OUTPUT_3D_API);
    m_rayCaster = std::make_unique<hvvr::Raycaster>(spec);

    loadScene(m_settings.initScene);
}
void ModelViewerLinux::onShutdown() {
    m_camera = nullptr;
    m_rayCaster = nullptr;
}
void ModelViewerLinux::onUserInput() {
    glfwPollEvents();
}
void ModelViewerLinux::onSimulation(double sceneTime, double deltaTime) {

}
void ModelViewerLinux::onRender() {
    m_camera->setCameraToWorld(m_cameraControl.toTransform());
    m_rayCaster->render(m_prevElapsedTime);
}
void ModelViewerLinux::onLoadScene(SceneSpecification spec) {
    std::cout << "Load scene" << std::endl;

    m_cameraControl.pos = spec.defaultCameraOrigin;
    m_cameraControl.yaw = spec.defaultCameraYaw;
    m_cameraControl.pitch = spec.defaultCameraPitch;
    float sceneScale = spec.scale;
    std::string scenePath = m_settings.sceneBasePath + spec.filename;

    // add a default directional light
    hvvr::LightUnion light = {};
    light.type = hvvr::LightType::directional;
    light.directional.Direction = hvvr::normalize(hvvr::vector3(-.25f, 1.0f, 0.1f));
    light.directional.Power = hvvr::vector3(0.4f, 0.35f, 0.35f);
    m_rayCaster->createLight(light);

    // load the scene
    std::cout << "Import model" << std::endl;
    model_import::Model importedModel;
    if (!model_import::load(scenePath.c_str(), importedModel)) {
        hvvr::fail("failed to load model %s", scenePath.c_str());
    }

    // apply scaling
    for (auto& mesh : importedModel.meshes) {
        mesh.transform.scale *= sceneScale;
    }

    std::cout << "Create objects in raycaster" << std::endl;
    // create the scene objects in the raycaster
    if (!model_import::createObjects(*m_rayCaster, importedModel)) {
        hvvr::fail("failed to create model objects");
    }
}
void ModelViewerLinux::onAfterLoadScene() {
    // Setup a regular camera
    m_camera = m_rayCaster->createCamera(hvvr::FloatRect(hvvr::vector2(-1, -1), hvvr::vector2(1, 1)),
                                         m_cameraSettings.lensRadius);
    m_camera->setFocalDepth(m_cameraSettings.focalDistance);

    int width;
    int height;
    glfwGetWindowSize(m_window, &width, &height);

    m_resizeCallback(width, height); // make sure we bind a render target and some samples to the camera
}
void ModelViewerLinux::endFrame() {
    // collect some overall perf statistics
    {
        struct FrameStats {
            float deltaTime;
            uint32_t rayCount;
        };
        enum { frameStatsWindowSize = 64 };
        static FrameStats frameStats[frameStatsWindowSize] = {};
        static int frameStatsPos = 0;

        uint32_t rayCount = m_camera->getSampleCount();

        frameStats[frameStatsPos].deltaTime = (float)m_deltaTime;
        frameStats[frameStatsPos].rayCount = rayCount;

        // let it run for a bit before collecting numbers
        if (frameStatsPos == 0 && m_frameID > frameStatsWindowSize * 4) {
            static double frameTimeAvgTotal = 0.0;
            static uint64_t frameTimeAvgCount = 0;

            // search for the fastest frame in the history window which matches the current state of the raycaster
            int fastestMatchingFrame = -1;
            for (int n = 0; n < frameStatsWindowSize; n++) {
                frameTimeAvgTotal += frameStats[n].deltaTime;
                frameTimeAvgCount++;

                if (fastestMatchingFrame == -1 ||
                    frameStats[n].deltaTime < frameStats[fastestMatchingFrame].deltaTime) {
                    fastestMatchingFrame = n;
                }
            }
            assert(fastestMatchingFrame >= 0 && fastestMatchingFrame < frameStatsWindowSize);
            const FrameStats& fastestFrame = frameStats[fastestMatchingFrame];

            float frameTimeAvg = float(frameTimeAvgTotal / double(frameTimeAvgCount));

            printf("%.0f (%.0f) mrays/s"
                   ", %.2f (%.2f) ms frametime"
                   ", %u x %u rays"
                   "\n",
                   fastestFrame.rayCount / fastestFrame.deltaTime / 1000000.0f * hvvr::COLOR_MODE_MSAA_RATE,
                   fastestFrame.rayCount / frameTimeAvg / 1000000.0f * hvvr::COLOR_MODE_MSAA_RATE,
                   fastestFrame.deltaTime * 1000, frameTimeAvg * 1000, fastestFrame.rayCount,
                   hvvr::COLOR_MODE_MSAA_RATE);
        }
        frameStatsPos = (frameStatsPos + 1) % frameStatsWindowSize;
    }

    m_frameID++;
}
void ModelViewerLinux::setResizeCallback(std::function<void(int, int)> callback) {
    m_resizeCallback = callback;
}

void ModelViewerLinux::createGLWindow() {
    // Create window
    glfwSetErrorCallback([](auto errorCode, auto message) {
        std::cout << "GLFW error code " << errorCode << ". " << message << std::endl;
    });

    if (!glfwInit()) {
        glfwTerminate();
        throw std::runtime_error("GLFW failed to initialize.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(RT_WIDTH, RT_HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow) {
        glfwTerminate();
        throw std::runtime_error("GLFW failed to create window");
    }

    // Get buffer size
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW
    glfwMakeContextCurrent(mainWindow);

    // Allow extensions
    glewExperimental = GL_TRUE;

    auto err = glewInit();
    if (err != GLEW_OK) {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        auto errString = glewGetErrorString(err);
        throw std::runtime_error(std::string{"GLEW failed to initialize: "} +
                                 std::string(reinterpret_cast<const char*>(errString)));
    }

    m_window = mainWindow;

    // Setup viewport
    glViewport(0, 0, bufferWidth, bufferHeight);
}
void ModelViewerLinux::run() {
    while (!glfwWindowShouldClose(m_window)) {
        // Get + Handle user input events
        double elapsedTime = m_timer.getElapsed();
        m_deltaTime = elapsedTime - m_prevElapsedTime;
        m_deltaTime = elapsedTime - m_prevElapsedTime;
        m_prevElapsedTime = elapsedTime;

        onUserInput();
        onSimulation(m_prevElapsedTime, m_deltaTime);
        onRender();
        endFrame();
    }
    onShutdown();
}
