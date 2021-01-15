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
        virtual bool getInterpMoveType();
        virtual void onRegist();
        virtual void onUnregist();
        virtual void onDeactive();
        virtual bool isOverwritePushInterpolation();
        virtual bool isOverwritePopInterpolation();
        virtual void getInterpolateFrame();
        virtual f32 calcInterpolation();
        virtual __int64 *push(__int64); // Cmn::CameraMgr::LayerId
        virtual __int64 *push(__int64 *); // Cmn::CameraMgr *
        virtual void swap(__int64, ushort); // Cmn::CameraMgr::LayerId
        virtual void swap(__int64 *, ushort); // Cmn::CameraMgr *
        virtual __int64 *pop(); // Cmn::CameraMgr *
        virtual __int64 *popIgnoreInterp(); // Cmn::CameraMgr *
        virtual __int64 *isActive(); // Cmn::CameraMgr *
        virtual bool isRegisted(); // nice one nintendo
        virtual Cmn::CameraBase *getCameraPtr();
        virtual Cmn::CameraBase *getCamera();
        virtual __int64 getProjection();
        virtual Cmn::CameraBase *getPos();
        virtual Cmn::CameraBase *getUp();
        virtual Cmn::CameraBase *getAt();
        virtual f32 getDir(__int64 *); // Lp::Utl::Math::DIR *
        virtual void getRight(sead::Vector3<f32> *)const;
        virtual f32 getBackword(sead::Vector3<f32> *)const; // nice one nintendo
        virtual void getForward(sead::Vector3<f32> *)const;
        virtual f32 getLeft(sead::Vector3<f32> *)const;
        virtual f32 getDown(sead::Vector3<f32> *)const;
        virtual f32 getFovy();
        virtual void getNear();
        virtual void getFar();
        virtual __int64 *getOffset(); // idk what this returns
        virtual f32 getOffsetX();
        virtual f32 getOffsetY();
        virtual f32 getAspect();
        virtual f32 getMtx(sead::Matrix34<f32> *);
        virtual Cmn::CameraBase *getAudioTargetPos();
        virtual void setProjection(f32, f32, f32);
        virtual void setPos(sead::Vector3<f32> const&);
        virtual void setAt(sead::Vector3<f32> const&);
        virtual void setUp(sead::Vector3<f32> const&);
        virtual void setDir(__int64 const&); // Lp::Utl::Math::DIR const&
        virtual void setFovy(f32);
        virtual void setNear(f32);
        virtual void setFar(f32);
        virtual void setOffset(sead::Vector2<f32>);
        virtual void setOffsetX(f32);
        virtual void setOffsetY(f32);
        virtual void setAspect(f32);
        virtual void setMtx(sead::Matrix34<f32> const&);
        virtual void setAudioTargetPos(sead::Vector3<f32> const&);
        virtual void setAudioTargetPosDefault();
        virtual __int64 calcAudioTargetPos(sead::Vector3<f32> const&); // idk what this returns
        virtual void lookAt(sead::Vector3<f32> const&, sead::Vector3<f32> const&, sead::Vector3<f32> const&);
        virtual void setOrthoProjectionMode();
        virtual void setOrthoTBLR(f32, int, int, int);
        virtual __int64 calc(); // idk what this returns
        virtual void regist(__int64 *); // Cmn::CameraMgr *
        virtual void unregist(); // nice one nintendo
        virtual void activate();
        virtual void deactivate();
        virtual void drawDebug(sead::Color4f const&, f32);
        virtual void genMessage(__int64 *); // Lp::Sys::HioContext *
        virtual void listenPropertyEvent(__int64 const*); // Lp::Sys::HioPropertyEvent const*
        virtual void listenNodeEvent(__int64 const*); // Lp::Sys::HioNodeEvent const*
        virtual void updateParam();
        virtual bool checkDerivedRuntimeTypeInfo(__int64 const*)const; // sead::RuntimeTypeInfo::Interface const*
        virtual __int64 *getRuntimeTypeInfo(); // idk what this returns
        virtual void onCalc();
        virtual void onActive();
    };
};