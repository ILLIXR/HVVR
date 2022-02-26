//
// Created by steven on 1/22/22.
//

#ifndef HVVR_MODELVIEWERCONFIG_H
#define HVVR_MODELVIEWERCONFIG_H

#include "vector_math.h"
#define RT_WIDTH 1920
#define RT_HEIGHT 1080

// for foveated
#define GAZE_CURSOR_MODE_NONE 0  // eye direction is locked forward
#define GAZE_CURSOR_MODE_MOUSE 1 // eye direction is set by clicking the mouse on the window
#define GAZE_CURSOR_MODE GAZE_CURSOR_MODE_NONE

// disable camera movement (for benchmarking)
#define DISABLE_MOVEMENT 0
#define CAMERA_SPEED 3.0

// 0 = off, 1 = match monitor refresh, 2 = half monitor refresh
#define ENABLE_VSYNC 0

#define ENABLE_DEPTH_OF_FIELD 0

// you might also want to enable JITTER_SAMPLES in kernel_constants.h
#define ENABLE_FOVEATED 0

#define ENABLE_WIDE_FOV 0

enum ModelviewerScene {
    scene_home = 0,
    scene_bunny,
    scene_conference,
    scene_sponza,
    scene_bistro_interior,
    SceneCount
};
// which scene to load? Can be overwritten in the command line
static ModelviewerScene gSceneSelect = scene_sponza;

using hvvr::vector3;
struct SceneSpecification {
    vector3 defaultCameraOrigin;
    float defaultCameraYaw;
    float defaultCameraPitch;
    float scale;
    std::string filename;
};
static SceneSpecification gSceneSpecs[ModelviewerScene::SceneCount] = {
    {vector3(1.0f, 3.0f, -1.5f), -(float)M_PI * .7f, (float)M_PI * -.05f, 1.0f, "oculus_home.bin"}, // Oculus Home
    {vector3(-0.253644f, 0.577575f, 1.081316f), -0.162111f, -0.453079f, 1.0f, "bunny.bin"},         // Stanford Bunny
    {vector3(10.091616f, 4.139270f, 1.230567f), -5.378105f, -0.398078f, 1.0f, "conference.bin"},    // Conference Room
    {vector3(4.198845f, 6.105420f, -0.400903f), -4.704108f, -0.200078f, .01f, "sponza.bin"},        // Crytek Sponza
    {vector3(2.0f, 2.0f, -0.5f), -(float)M_PI * .5f, (float)M_PI * -.05f, 1.0f, "bistro.bin"}       // Amazon Bistro
};

struct CameraSettings {
    float lensRadius = (ENABLE_DEPTH_OF_FIELD == 1) ? 0.0015f : 0.0f;
    float focalDistance = 0.3f;
    bool foveatedCamera = (ENABLE_FOVEATED == 1);
    bool movable = (DISABLE_MOVEMENT == 0);
    float maxSpeed = (float)CAMERA_SPEED;
};

struct CameraControl {
    vector3 pos = {};
    float yaw = 0.0f;
    float pitch = 0.0f;
    void locallyTranslate(vector3 delta) {
        pos += hvvr::matrix3x3(hvvr::quaternion::fromEulerAngles(yaw, pitch, 0)) * delta;
    }
    hvvr::transform toTransform() const {
        return hvvr::transform(pos, hvvr::quaternion::fromEulerAngles(yaw, pitch, 0), 1.0f);
    }
};

#endif // HVVR_MODELVIEWERCONFIG_H
