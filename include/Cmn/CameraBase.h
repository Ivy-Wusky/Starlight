#pragma once

#include "types.h"
#include "sead/vector.h"
#include "sead/matrix.h"
#include "sead/string.h"
#include "sead/color.h"
#include "sead/camera.h"

namespace Cmn {
    class CameraBase : sead::Camera {
        public:
        CameraBase(sead::SafeStringBase<char> const&, bool);
        ~CameraBase();
        bool getInterpMoveType();
        void onRegist();
        void onUnregist();
        void onDeactive();
        bool isOverwritePushInterpolation();
        bool isOverwritePopInterpolation();
        void getInterpolateFrame();
        f32 calcInterpolation();
        __int64 *push(__int64); // Cmn::CameraMgr::LayerId
        __int64 *push(__int64 *); // Cmn::CameraMgr *
        void swap(__int64, ushort); // Cmn::CameraMgr::LayerId
        void swap(__int64 *, ushort); // Cmn::CameraMgr *
        __int64 *pop(); // Cmn::CameraMgr *
        __int64 *popIgnoreInterp(); // Cmn::CameraMgr *
        __int64 *isActive(); // Cmn::CameraMgr *
        bool isRegisted(); // nice one nintendo
        Cmn::CameraBase *getCameraPtr();
        Cmn::CameraBase *getCamera();
        __int64 getProjection();
        Cmn::CameraBase *getPos();
        Cmn::CameraBase *getUp();
        Cmn::CameraBase *getAt();
        f32 getDir(__int64 *); // Lp::Utl::Math::DIR *
        void getRight(sead::Vector3<f32> *)const;
        f32 getBackword(sead::Vector3<f32> *)const; // nice one nintendo
        void getForward(sead::Vector3<f32> *)const;
        f32 getLeft(sead::Vector3<f32> *)const;
        f32 getDown(sead::Vector3<f32> *)const;
        f32 getFovy();
        void getNear();
        void getFar();
        __int64 *getOffset(); // idk what this returns
        f32 getOffsetX();
        f32 getOffsetY();
        f32 getAspect();
        f32 getMtx(sead::Matrix34<f32> *);
        Cmn::CameraBase *getAudioTargetPos();
        void setProjection(f32, f32, f32);
        void setPos(sead::Vector3<f32> const&);
        void setAt(sead::Vector3<f32> const&);
        void setUp(sead::Vector3<f32> const&);
        void setDir(__int64 const&); // Lp::Utl::Math::DIR const&
        void setFovy(f32);
        void setNear(f32);
        void setFar(f32);
        void setOffset(sead::Vector2<f32>);
        void setOffsetX(f32);
        void setOffsetY(f32);
        void setAspect(f32);
        void setMtx(sead::Matrix34<f32> const&);
        void setAudioTargetPos(sead::Vector3<f32> const&);
        void setAudioTargetPosDefault();
        __int64 calcAudioTargetPos(sead::Vector3<f32> const&); // idk what this returns
        void lookAt(sead::Vector3<f32> const&, sead::Vector3<f32> const&, sead::Vector3<f32> const&);
        void setOrthoProjectionMode();
        void setOrthoTBLR(f32, int, int, int);
        __int64 calc(); // idk what this returns
        void regist(__int64 *); // Cmn::CameraMgr *
        void unregist(); // nice one nintendo
        void activate();
        void deactivate();
        void drawDebug(sead::Color4f const&, f32);
        void genMessage(__int64 *); // Lp::Sys::HioContext *
        void listenPropertyEvent(__int64 const*); // Lp::Sys::HioPropertyEvent const*
        void listenNodeEvent(__int64 const*); // Lp::Sys::HioNodeEvent const*
        void updateParam();
        bool checkDerivedRuntimeTypeInfo(__int64 const*)const; // sead::RuntimeTypeInfo::Interface const*
        __int64 *getRuntimeTypeInfo(); // idk what this returns
        void onCalc();
        void onActive();
    };
};