//
// Created by steven on 1/21/22.
//

#ifndef HVVR_MODELVIEWERLINUX_H
#define HVVR_MODELVIEWERLINUX_H

#include "ModelViewerConfig.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cuda_runtime.h>
#include <functional>
#include "timer.h"
#include "raycaster.h"

class ModelViewerLinux {
public:
    enum OutputMode { OUTPUT_NONE, OUTPUT_3D_API };
    struct Settings {
        std::string windowName = "Modelviewer";
        uint32_t width = RT_WIDTH;
        uint32_t height = RT_HEIGHT;
        std::string sceneBasePath = "./scenes/";
        // 0 = off, 1 = match monitor refresh, 2 = half monitor refresh
        int vSync = ENABLE_VSYNC;
        SceneSpecification initScene;
        OutputMode outputMode = OUTPUT_3D_API;
    };
    ModelViewerLinux(Settings settings);
    void createGLWindow();
    void createRenderTexture();
    void onInit();
    void onShutdown();
    void onUserInput();
    void onSimulation(double sceneTime, double deltaTime);
    void onRender();
    void onLoadScene(SceneSpecification spec);
    void onAfterLoadScene();
    void loadScene(SceneSpecification spec) {
        onLoadScene(spec);
        onAfterLoadScene();
    }
    void endFrame();


    void setResizeCallback(std::function<void(int, int)> callback);

    // Run until we get a quit message, on which we return
    void run();

private:
    Settings m_settings;
    hvvr::Timer m_timer;

    GLFWwindow* m_window = nullptr;
    std::unique_ptr<hvvr::Raycaster> m_rayCaster;

    std::function<void(int, int)> m_resizeCallback;

    double m_prevElapsedTime;
    double m_deltaTime;

    uint64_t m_frameID = 0;
    hvvr::Camera* m_camera = nullptr;
    CameraControl m_cameraControl = {};

    CameraSettings m_cameraSettings;

    // OpenGL render texture
    GLuint m_renderTexture;
    GLuint m_textureFBO;
};

#endif // HVVR_MODELVIEWERLINUX_H
