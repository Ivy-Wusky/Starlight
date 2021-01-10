#include "main.hpp"

using namespace starlight;

// Needed on old versions of rtld that doesn't check for DT_INIT existance.
extern "C" void __custom_init(void) {}

// DT_FINI here for completeness.
extern "C" void __custom_fini(void) {}

static agl::DrawContext *mDrawContext;
static sead::TextWriter *mTextWriter;
static sead::ExpHeap* mStarlightHeap;
static sead::Thread* mLoggingThread;
static View* mView;
static int mode;
static bool showMenu;
static int nnSocketInit = false;
const char* validMemIdentityCheck = "03dbbed2e04e31b0e9081c08a65ee4fb";
static bool validateMemIdentityCheck = true;

uint32_t readU32(uint32_t *p, uint32_t offset)
{
    uint32_t res;
    asm volatile("LDR %[result], [%[base], %[offset]]"
                 : [ result ] "=r"(res)
                 : [ base ] "r"(p), [ offset ] "r"(offset)
                 :);
    return res;
}

uint64_t readU64(uint64_t *p, uint64_t offset)
{
    uint32_t res;
    asm volatile("LDR %[result], [%[base], %[offset]]"
                 : [ result ] "=r"(res)
                 : [ base ] "r"(p), [ offset ] "r"(offset)
                 :);
    return res;
}

/*
uint32_t getGeyserPos(uint64_t pos, uint64_t first) {
    if (pos == first || pos == 0) return 65;
    return (pos - first - 0x6B0) / 0x900 + 65;
}
*/

void renderEntrypoint(agl::DrawContext *drawContext, sead::TextWriter *textWriter)
{
    mDrawContext = drawContext;
    mTextWriter = textWriter;
    Cmn::MemIdentityChecker* memIdentityChecker = Collector::mMemIdentityCheckerInstance;

    mTextWriter->mColor = sead::Color4f::cWhite;

    Collector::init();
    Collector::collect();

    if(mStarlightHeap == NULL)
        allocHeap();
    if(mStarlightHeap != NULL)
        Collector::mHeapMgr->setCurrentHeap_(mStarlightHeap);

    if (memIdentityChecker != NULL && validateMemIdentityCheck == true) {
        memcpy(memIdentityChecker->mHash.mCharPtr, validMemIdentityCheck, 32);
    }

    if(Collector::mController.isPressed(Controller::Buttons::Minus1))
        showMenu = !showMenu;

    static bool init = false;
    if(!init){
        TcpLogger::Log("renderEntrypoint INIT.\n");

        sead::SafeStringBase<char> loggingThreadName;
        loggingThreadName.mCharPtr = "starlight::LoggingThread";
        sead::Delegate2<sead::TaskMgr, sead::Thread *,s64> loggingDelegate;

        loggingDelegate.mCallback = loggerMain;

        mLoggingThread = new sead::DelegateThread(loggingThreadName, &loggingDelegate, mStarlightHeap, 4, sead::MessageQueue::BlockType::DEFAULT, 0x7FFFFFFF, 0x10000, 20);
        mLoggingThread->start();
        TcpLogger::Log("Logging thread started.\n");

        mView = new View();
        menu::SimpleMenu* m = new menu::SimpleMenu();

        auto loadMushCallback = [](starlight::View* view) {
            view->pushMenu(new menu::MushViewMenu());
        };

        menu::SimpleMenuEntry* heapNameEntry = new menu::SimpleMenuEntry();
        menu::SimpleMenuEntry* modeDisplayEntry = new menu::SimpleMenuEntry();
        menu::SimpleMenuEntry* sceneDisplayEntry = new menu::SimpleMenuEntry();
        menu::SimpleMenuEntry* menuTitleDisplayEntry = new menu::SimpleMenuEntry();
        heapNameEntry->mRenderCallback = [](){
            return "Current Heap Name: " + std::string(Collector::mHeapMgr->getCurrentHeap()->mName.mCharPtr); };
        modeDisplayEntry->mRenderCallback = [](){
            return "Current Mode: " + std::string(modeToText((Modes)mode)); };
        sceneDisplayEntry->mRenderCallback = [](){ 
            return "Current Scene Name: " + std::string(Lp::Utl::getCurSceneName()); };
        menuTitleDisplayEntry->mRenderCallback = [](){
            return "Starleon"; };
        
        menu::SimpleMenuEntry* mushEntry = new menu::SimpleMenuEntry();
        mushEntry->mSelectedCallback = loadMushCallback;
        mushEntry->mRenderCallback = []() { return "Mush Viewer"; };

        m->mEntries.push_back(mushEntry);
        m->mEntries.push_back(heapNameEntry);
        m->mEntries.push_back(modeDisplayEntry);
        m->mEntries.push_back(sceneDisplayEntry);
        m->mEntries.push_back(menuTitleDisplayEntry);

        mView->pushMenu(m);
        
        init = true;

        TcpLogger::Log("Starlight INIT complete.\n");
    }

    /*
    Application* app;
    textWriter->printf("FPS: %f\n", app->_28->calcFps());

    aal::Debugger* dgber = new aal::Debugger();
    dgber->drawInformation(drawContext, textWriter);
    */
    
    // textWriter->printf("Current Heap Name: %s\n", Collector::mHeapMgr->getCurrentHeap()->mName.mCharPtr);
    textWriter->printf("Current Heap Free Space: 0x%x\n", Collector::mHeapMgr->getCurrentHeap()->getFreeSize());

    mView->update();
    mView->render(mTextWriter);

    if(showMenu){
        // drawBackground();
        
        textWriter->setScaleFromFontHeight(20);
        sead::TextWriter::setupGraphics(drawContext); // re-setup context

        /*
        textWriter->printf("Welcome to Starlight!\n");
        textWriter->printf("This is a demonstration of C/C++ code running in the context of a Switch game!\n");
        textWriter->printf("Credit to shibboleet, Khangaroo, Thog, Retr0id, and the libnx maintainers!\n");
        */
        
        // textWriter->printf("Current Scene Name: %s\n", Lp::Utl::getCurSceneName());

        if(Collector::mController.isPressed(Controller::Buttons::RStick))
            mode++;
        if(mode > Modes::END)
            mode = 0;
        // textWriter->printf("Current Mode: %s\n", modeToText((Modes)mode));

        Cmn::StaticMem *staticMem = Collector::mStaticMemInstance;
        if(staticMem != NULL)
            handleStaticMem(staticMem);

        Game::PlayerMgr *playerMgr = Collector::mPlayerMgrInstance;
        if(playerMgr != NULL)
            handlePlayerMgr(playerMgr);
            
        Cmn::PlayerCtrl *playerCtrl = Collector::mPlayerCtrlInstance;
        if(playerCtrl != NULL)
            handlePlayerControl(playerCtrl);
        else if(mode == Modes::INPUT_VIEWER){
            mTextWriter->printf("Information not available.\n");
        }

        Cmn::MushDataHolder* mushData = Collector::mMushDataHolder;
        if(mushData != NULL)
            handleMushDataHolder(mushData);

        Game::MainMgr* mainMgr = Collector::mMainMgrInstance;
        if(mainMgr != NULL){
            handleMainMgr(mainMgr);
        }
    }

    Collector::mController.update();
    Collector::mHeapMgr->setCurrentHeap_(NULL);
}

void allocHeap() {
    Lp::Sys::HeapGroup* heapGroup = Lp::Sys::HeapGroup::sInstance;
    if(heapGroup != NULL){
        sead::ExpHeap* othersHeap = heapGroup->mHeaps[Lp::Sys::HeapGroup::Group::cOthers];
        Collector::mHeapMgr->setCurrentHeap_(othersHeap);
        sead::Heap* currentHeap = Collector::mHeapMgr->getCurrentHeap();

        if(mStarlightHeap == NULL){
            sead::SafeStringBase<char> str;
            str.mCharPtr = (char*)"StarlightHeap";

            mStarlightHeap = sead::ExpHeap::create(0, str, currentHeap, 4, sead::Heap::HeapDirection::TAIL, true);
        }
    }
}

void hooks_init() {
    isCreateMantleHook();
    memIdentityCheckHook(NULL);
}

nn::os::Tick nnSocketInitHook(){
    nnSocketInit++;
    return nn::os::GetSystemTick(); // expected result of replaced call
}

bool isCreateMantleHook() {
    return true;
}

void memIdentityCheckHook(Cmn::MemIdentityChecker *memIdentityChecker) {
    if (memIdentityChecker != NULL) {
        if (validateMemIdentityCheck == true) {
            memcpy(memIdentityChecker->mHash.mCharPtr, validMemIdentityCheck, 32);
        }
        else {
            memIdentityChecker->calcHash();
        }
    }
}

void loggerMain(sead::Thread* thread, s64){
    Collector::init();
    Collector::collect();

    Collector::mHeapMgr->setCurrentHeap_(mStarlightHeap);

    while(nnSocketInit != 2) {
        Lp::Utl::sleepCurThreadMilliSec(10);
    };

    TcpLogger::Initialize();

    while(true){
        TcpLogger::ClearQueue();
        Lp::Utl::sleepCurThreadMilliSec(1);
    }
}

void drawBackground(){
    sead::Vector3<float> p1; // top left
    p1.mX = -1.0;
    p1.mY = 1.0;
    p1.mZ = 0.0;
    sead::Vector3<float> p2; // top right
    p2.mX = 0.0;
    p2.mY = 1.0;
    p2.mZ = 0.0;
    sead::Vector3<float> p3; // bottom left
    p3.mX = -1.0;
    p3.mY = -1.0;
    p3.mZ = 0.0;
    sead::Vector3<float> p4; // bottom right
    p4.mX = 0.0;
    p4.mY = -1.0;
    p4.mZ = 0.0;

    sead::Color4f c;
    c.r = 1.0;
    c.g = 1.0;
    c.b = 1.0;
    c.a = 0.5;

    agl::utl::DevTools::beginDrawImm(mDrawContext, sead::Matrix34<float>::ident, sead::Matrix44<float>::ident);
    agl::utl::DevTools::drawTriangleImm(mDrawContext, p1, p2, p3, c);
    agl::utl::DevTools::drawTriangleImm(mDrawContext, p3, p4, p2, c);
}

void handleStaticMem(Cmn::StaticMem *staticMem){
    sead::SafeStringBase<char> *stageName = &staticMem->mMapFileName[0];
    if(stageName->mCharPtr != NULL){
        mTextWriter->printf("Loaded Stage: %s\n", stageName->mCharPtr);
    }
    
    Cmn::PlayerInfoAry *playerInfoAry = staticMem->mPlayerInfoAry;
    if(playerInfoAry != NULL){
        mTextWriter->printf("PlayerInfoAry PTR: 0x%x\n", playerInfoAry);
    }
}

void handlePlayerMgr(Game::PlayerMgr* playerMgr){
    Game::Player* player = playerMgr->getControlledPerformer();
    if (player != NULL) {
        if (mode == Modes::TEAM_COLOR_PICKER) {
            static long choice = 0;

            if (Collector::mController.isPressed(Controller::Buttons::UpDpad))
                choice++;
            if (Collector::mController.isPressed(Controller::Buttons::DownDpad))
                choice--;
            
            if (choice < 0)
                choice = 0;

            if (choice > 9)
                choice = 0;

            switch (choice)
            {
                case 0:
                    mTextWriter->printf("Current Team Color: Default\n");
                    break;
                case 1:
                    player->replaceTeamColor(sead::Color4f::cBlack);
                    mTextWriter->printf("Current Team Color: Black\n");
                    break;
                case 2:
                    player->replaceTeamColor(sead::Color4f::cBlue);
                    mTextWriter->printf("Current Team Color: Blue\n");
                    break;
                case 3:
                    player->replaceTeamColor(sead::Color4f::cCyan);
                    mTextWriter->printf("Current Team Color: Cyan\n");
                    break;
                case 4:
                    player->replaceTeamColor(sead::Color4f::cGray);
                    mTextWriter->printf("Current Team Color: Gray\n");
                    break;
                case 5:
                    player->replaceTeamColor(sead::Color4f::cGreen);
                    mTextWriter->printf("Current Team Color: Green\n");
                    break;
                case 6:
                    player->replaceTeamColor(sead::Color4f::cMagenta);
                    mTextWriter->printf("Current Team Color: Magenta\n");
                    break;
                case 7:
                    player->replaceTeamColor(sead::Color4f::cRed);
                    mTextWriter->printf("Current Team Color: Red\n");
                    break;
                case 8:
                    player->replaceTeamColor(sead::Color4f::cWhite);
                    mTextWriter->printf("Current Team Color: White\n");
                    break;
                case 9:
                    player->replaceTeamColor(sead::Color4f::cYellow);
                    mTextWriter->printf("Current Team Color: Yellow\n");
                    break;
                default:
                    mTextWriter->printf("Current Team Color: Default\n");
                    break;
            }
        }
    }

    Cmn::PlayerInfo* info = player->mPlayerInfo;
    if(info != NULL){
        // mTextWriter->printf("Controlled Player Team: %x\n", info->mTeam);
        mTextWriter->printf(u"Controlled Player Name: %s\n", info->mPlayerName);
        if(info->mPlayerIndex == 0){
            info->setPlayerName(u"Ivy ");
        }
    }

    if (playerMgr != NULL && player != NULL) {
        if (mode == Modes::TEAM_SWITCHER) {
            if (info->mTeam == Cmn::Def::Team::Alpha) {
                mTextWriter->printf("Controlled Player Team: Alpha\n");
            }
            else if (info->mTeam == Cmn::Def::Team::Bravo) {
                mTextWriter->printf("Controlled Player Team: Bravo\n");
            }

            /*
            if (Collector::mController.isPressed(Controller::Buttons::UpDpad)) {
                player->mPlayerInfo->mTeam = bool(player->mPlayerInfo->mTeam) ^ 1;
                player->mTeam = bool(readU64(uint64_t(player), 0x328)) ^ 1;
            }
            */
        }
    }

    Game::PlayerMotion *playerMotion = player->mPlayerMotion;

    if(mode == Modes::EVENT_VIEWER) {
        static long scroll = 0;

        if(Collector::mController.isPressed(Controller::Buttons::UpDpad))
            scroll++;
        if(Collector::mController.isPressed(Controller::Buttons::DownDpad))
            scroll--;
            
        if(scroll < 0)
            scroll = 0;
        
        playerMotion->startEventAnim((Game::PlayerMotion::AnimID) scroll, 0, 1.0);
        mTextWriter->printf("Event ID: 0x%x\n", scroll);

    } else if(mode == Modes::PLAYER_SWITCHER){

        signed int currentPlayer = playerMgr->mCurrentPlayerIndex;
        mTextWriter->printf("Current Player: %i\n", currentPlayer);

        if(Collector::mController.isPressed(Controller::Buttons::UpDpad))
            currentPlayer++;
        if(Collector::mController.isPressed(Controller::Buttons::DownDpad))
            currentPlayer--;

        if(currentPlayer < 0)
            currentPlayer = playerMgr->mTotalPlayerArry.mBufferSize;
        if(playerMgr->mTotalPlayerArry.mBufferSize <= currentPlayer)
            currentPlayer = 0;
        
        playerMgr->mCurrentPlayerIndex = currentPlayer;
        playerMgr->onChangeControlledPlayer();
    }

    // playerMgr->updateAllControlledPlayer_(0);

    // Cmn::IPlayerCustomInfo* playerCustomInfo = Collector::mPlayerCustomInfo;
    Cmn::MemIdentityChecker* memIdentityChecker = Collector::mMemIdentityCheckerInstance;
    if (memIdentityChecker != NULL /* && playerCustomInfo != NULL */) {
        if (mode == Modes::HOOK_STUFF) {
            // mTextWriter->printf("isCreateMantle: %s", playerCustomInfo->isCreateMantle() ? "true" : "false");
            mTextWriter->printf("isCreateMantle: %s", player->isCreateMantle() ? "true" : "false");
            mTextWriter->printf("mHash: %s", memIdentityChecker->mHash.mCharPtr);
        }
    }
}

void handlePlayerControl(Cmn::PlayerCtrl* playerCtrl){
    Game::PlayerGamePadData::FrameInput input;
    input.record(); // grab input data

    if(mode == Modes::INPUT_VIEWER){
        mTextWriter->printf("Left Stick | X: %f | Y: %f\n", input.leftStick.mX, input.leftStick.mY);
        mTextWriter->printf("Right Stick | X: %f | Y: %f\n", input.rightStick.mX, input.rightStick.mY);
        mTextWriter->printf("Angle Vel | X: %f | Y: %f | Z: %f\n", input.angleVel.mX, input.angleVel.mY, input.angleVel.mZ);
        mTextWriter->printf("Posture X | X: %f | Y: %f | Z: %f\n", input.postureX.mX, input.postureX.mY, input.postureX.mZ);
        mTextWriter->printf("Posture Y | X: %f | Y: %f | Z: %f\n", input.postureY.mX, input.postureY.mY, input.postureY.mZ);
        mTextWriter->printf("Posture Z | X: %f | Y: %f | Z: %f\n", input.postureZ.mX, input.postureZ.mY, input.postureZ.mZ);
    }

    static bool entered = false;
    Game::Player* player = Collector::mControlledPlayer;
    if(mode == 1 && player != NULL){
        static float x, y, z;
        sead::Vector3<float> *playerPos = &player->mPosition;
        if(!entered){
            x = playerPos->mX;
            y = playerPos->mY;
            z = playerPos->mZ;
        }

        int speed = 10;

        if(Collector::mController.isHeld(Controller::Buttons::UpDpad))
            y+=speed;
        if(Collector::mController.isHeld(Controller::Buttons::DownDpad))
            y-=speed;
        if(Collector::mController.isHeld(Controller::Buttons::LeftDpad))
            x+=speed;
        if(Collector::mController.isHeld(Controller::Buttons::RightDpad))
            x-=speed;
        if(Collector::mController.isHeld(Controller::Buttons::RightRStickOrdinal))
            z+=speed;
        if(Collector::mController.isHeld(Controller::Buttons::LeftRStickOrdinal))
            z-=speed;

        playerPos->mX = x;
        playerPos->mY = y;
        playerPos->mZ = z;
        entered = true;
    } else {
        entered = false;
    }
}

void handleMushDataHolder(Cmn::MushDataHolder* mushDataHolder){
    static bool entered = false;

    if(!entered){
        for(int i = 0; i < 29001; i++){
            Cmn::WeaponData* data = mushDataHolder->mMushWeaponInfo->getById(Cmn::Def::WeaponKind::cMain, i);
            if(data != NULL){
                data->mPrice = 0;
                data->mRank = 0;
                data->mSpecialCost = 0;
                data->mLockType = Cmn::WeaponData::LockType::None;
                // data->mDoubleType = Cmn::WeaponData::DoubleType::Mirror;
            }
        }

        for(int i = 0; i < 230; i++){
            Cmn::MushMapInfo::Data* mapData = mushDataHolder->mMushMapInfo->getByMushOrder(i);
            if(strcmp(mapData->mStr1.mCharPtr, "Fld_Plaza00_Plz") != 0) // plaza crashes when using night mode for whatever reason
                if(mapData != NULL){
                    mapData->mEnvHour = 2;
                }
        }

        entered = true;
    }
}

void handleMainMgr(Game::MainMgr* mainMgr) {
    Game::GfxMgr* gfxMgr = mainMgr->gfxMgr;
    if(gfxMgr != NULL){
        gfxMgr->hour = 2;
    }
    if(mode == Modes::PAINT_ALL){
        if(Collector::mController.isPressed(Controller::Buttons::LStick)){
            unsigned int paintGameFrame = mainMgr->getPaintGameFrame();
            Cmn::Def::Team team = Cmn::Def::Team::Alpha;
            if(Collector::mControlledPlayer != NULL)
                team = Collector::mControlledPlayer->mTeam;
            Game::PaintUtl::requestAllPaintFloor(paintGameFrame, team);
            Game::PaintUtl::requestAllPaintWall(paintGameFrame, team);
        }
    }
}

/*
bool isTriggered(Lp::Sys::Ctrl *controller, unsigned long id){
    bool buttonHeld = controller->data & id;
    return buttonHeld & !(controller->data & lastInputs & id);
}
*/

bool isSceneLoaded(char *name) {
    if (Lp::Utl::getCurSceneName() == name) {
        return true;
    }
    else if (Lp::Utl::getCurSceneName() != name) {
        return false;
    }
}

char const* modeToText(Modes mode){
    switch(mode){
        case Modes::NONE:
            return "None";
        case Modes::TEAM_COLOR_PICKER:
            return "Team Color Picker";
        case Modes::FLY:
            return "Fly";
        case Modes::EVENT_VIEWER:
            return "Event Viewer";
        case Modes::INPUT_VIEWER:
            return "Gyro/Stick Input Viewer";
        case Modes::PLAYER_SWITCHER:
            return "Player Switcher";
        case Modes::TEAM_SWITCHER:
            return "Team Switcher";
        case Modes::PAINT_ALL:
            return "Paint All";
        case Modes::HOOK_STUFF:
            return "Dev";
        default:
            return "None";
    }
}

int main(int arg, char **argv){
    if (svcGetSystemTick() > 0) {
        return 0;
    }
}