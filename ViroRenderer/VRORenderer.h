//
//  VRORenderer.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VRORenderer_h
#define VRORenderer_h

#include <memory>
#include <vector>
#include "VROVector3f.h"
#include "VROQuaternion.h"
#include "VROMatrix4f.h"
#include "VRORenderDelegate.h"
#include "VROFrameSynchronizer.h"

class VROEye;
class VRONode;
class VRODriver;
class VROCameraMutable;
class VROTimingFunction;
class VRORenderContext;
class VROViewport;
class VROFieldOfView;
class VROFrameListener;
enum class VROCameraRotationType;
enum class VROEyeType;
enum class VROTimingFunctionType;

@class VROScreenUIView; //TODO delete
@class VROSceneController;

class VRORenderer {
    
public:
    
    VRORenderer();
    virtual ~VRORenderer();
        
    void setPosition(VROVector3f position);
    void setBaseRotation(VROQuaternion quaternion);
    void setCameraRotationType(VROCameraRotationType type);
    void setOrbitFocalPoint(VROVector3f focalPt);
    
    float getWorldPerScreen(float distance, const VROFieldOfView &fov,
                            const VROViewport &viewport) const;
    void setDelegate(id <VRORenderDelegate> delegate);
    void updateRenderViewSize(CGSize size);
    
#pragma mark - Scene Controllers
    
    void setSceneController(VROSceneController *sceneController, const VRODriver &driver);
    void setSceneController(VROSceneController *sceneController, bool animated, const VRODriver &driver);
    void setSceneController(VROSceneController *sceneController, float seconds,
                            VROTimingFunctionType timingFunctionType, const VRODriver &driver);
    VROSceneController *getSceneController() const {
        return _sceneController;
    }
    
#pragma mark - Render Loop
    
    void prepareFrame(int frame, VROMatrix4f headRotation, VRODriver &driver);
    void renderEye(VROEyeType eye, VROMatrix4f eyeFromHeadMatrix, VROMatrix4f projectionMatrix,
                   const VRODriver &driver);
    void endFrame(const VRODriver &driver);
    
#pragma mark - Frame Synchronizer
    
    std::shared_ptr<VROFrameSynchronizer> getFrameSynchronizer() {
        return _frameSynchronizer;
    }

    void handleTap();
    VROScreenUIView *getHUD() {
        return _HUD;
    }
    
private:
    
    bool _rendererInitialized;
    
    /*
     Manages per-frame listeners.
     */
    std::shared_ptr<VROFrameSynchronizer> _frameSynchronizer;
    
    /*
     Maintains parameters used for scene rendering.
     */
    std::shared_ptr<VRORenderContext> _context;
    
    /*
     The screen-space view.
     */
    VROScreenUIView *_HUD;
    
    /*
     Internal representation of the camera.
     */
    std::shared_ptr<VROCameraMutable> _camera;
    
    /*
     Delegate receiving scene rendering updates.
     */
    id <VRORenderDelegate> _delegate; //TODO Make this not Obj-C, and weak ptr

#pragma mark - Scene and Scene Transitions
    
    VROSceneController *_sceneController;
    VROSceneController *_outgoingSceneController;

    bool _sceneTransitionActive;
    float _sceneTransitionDuration;
    float _sceneTransitionStartTime;
    std::unique_ptr<VROTimingFunction> _sceneTransitionTimingFunction;
    
    bool processSceneTransition();
    
#pragma mark - Scene Rendering
    
    void renderEye(VROEyeType eyeType, const VRODriver &driver);
    
#pragma mark - Frame Listeners
    
    void notifyFrameStart();
    void notifyFrameEnd();
    
};

#endif /* VRORenderer_h */
