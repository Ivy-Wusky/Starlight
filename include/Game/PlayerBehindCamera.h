#pragma once

#include "Cmn/CameraBase.h"
#include "Game/Player/Player.h"
#include "types.h"

namespace Game {
    class PlayerBehindCamera : Cmn::CameraBase {
        public:
        PlayerBehindCamera(Game::Player const*);
        ~PlayerBehindCamera();
        void setupGyro();
        __int64 calcPitchPos(f32, f32, f32, f32, f32, f32, f32);
        
    };
};