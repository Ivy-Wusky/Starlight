#pragma once

#include <string.h>
#include <vector>
#include <math.h>
#include "svc.h"
#include "types.h"
#include "operator.h"
#include "agl/lyr/renderinfo.h"
#include "agl/lyr/renderer.h"
#include "agl/utl/devtools.h"
#include "sead/textwriter.h"
#include "sead/string.h"
#include "sead/drawer.h"
#include "sead/heap.h"
#include "sead/matrix.h"
#include "sead/arena.h"
#include "sead/system.h"
#include "sead/thread.h"
#include "Cmn/StaticMem.h"
#include "Cmn/CameraUtl.h"
#include "Cmn/PlayerInfoUtil.h"
#include "Cmn/Player/PlayerCtrl.h"
#include "Cmn/Mush/MushDataHolder.h"
#include "Cmn/GfxSetting.h"
#include "Cmn/AppUBOMgr.h"
#include "Cmn/Def/util.h"
#include "Cmn/GfxUtl.h"
#include "Cmn/IPlayerCustomInfo.h"
#include "Cmn/MemIdentityChecker.h"
#include "Lp/Utl.h"
#include "Lp/Sys/ctrl.h"
#include "Lp/Sys/heapgroup.h"
#include "Game/Utl.h"
#include "Game/PlayerGamePadData/FrameInput.h"
#include "Game/Player/PlayerMotion.h"
#include "Game/Player/Player.h"
#include "Game/PlayerMgr.h"
#include "Game/MainMgr.h"
#include "Game/PaintUtl.h"
#include "Game/RivalMgr.h"
#include "MiniGame/gfx.h"
#include "aal/debugdrawer.h"
// #include "aal/Debugger.h"
#include "nn/hid.h"
#include "ModuleObject.hpp"
#include "starlight/collector.hpp"
#include "starlight/view.hpp"
#include "starlight/logger/TcpLogger.hpp"
#include "starlight/menu/simplemenu.hpp"
#include "starlight/menu/popmenuentry.hpp"
#include "starlight/menu/mushmenu.hpp"
// #include "system/Application.h"

enum Modes {
    NONE, TEAM_COLOR_PICKER, FLY, EVENT_VIEWER, INPUT_VIEWER,  PLAYER_SWITCHER, TEAM_SWITCHER, PAINT_ALL, HOOK_STUFF, END
};

void renderEntrypoint(agl::DrawContext *drawContext, sead::TextWriter *textWriter);

void allocHeap();

nn::os::Tick nnSocketInitHook();
void loggerMain(sead::Thread* thread, s64);

void drawBackground();
void handleStaticMem(Cmn::StaticMem *staticMem);
void handlePlayerMgr(Game::PlayerMgr* playerMgr);
void handlePlayerControl(Cmn::PlayerCtrl* playerCtrl);
void handleMushDataHolder(Cmn::MushDataHolder* mushDataHolder);
void handleMainMgr(Game::MainMgr* mainMgr);
void handleGfxSetting(Cmn::GfxSetting*);

char const* modeToText(Modes);

uint32_t readU32(uint32_t *p, uint32_t offset);
uint64_t readU64(uint64_t *p, uint64_t offset);
// uint32_t getGeyserPos(uint64_t pos, uint64_t first);

bool isSceneLoaded(char *name);
// bool isTriggered(Lp::Sys::Ctrl *controller, unsigned long id);

void hooks_init();
bool isCreateMantleHook();
void memIdentityCheckHook(Cmn::MemIdentityChecker *memIdentityChecker);