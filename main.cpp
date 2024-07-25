#include <include/RCRenderer.h>
#include <include/RCVideoWindow.h>
#include <include/RCInteractor.h>

#include <fstream>
#include <cmath>
#include <string>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	std::ifstream stream("./test.txt");
	int mapWidth;
	int mapHeight;

	RCCamera    camera;
	std::string temp;
	std::getline(stream, temp);
	mapWidth = atoi(temp.c_str());
	std::getline(stream, temp);
	mapHeight = atoi(temp.c_str());

	camera.Z = 0;
	camera.SetPitch(0.f);

	RCTexture testTexture(new RCContext(_T("./pics/cobblestone.png")));
	RCTexture testDigTexture(new RCContext(_T("./pics/kobi.jpg")));
	auto doorContext = new RCContext(_T("./pics/avatar.jpg"));
	RCTexture testDoorTexture(doorContext);
	auto glassContext = new RCContext(_T("./pics/glass.png"));
	RCTexture testGlassTexture(glassContext);
	auto stripContext = new RCContext(_T("./pics/strip.png"));
	RCTexture testStripTexture(stripContext);

	auto mapUnit = new RCMapUnit[mapWidth * mapHeight];
	int count = 0;
	int x = 0;
	int y = 0;
	int wallCount = 0;
	int *door;
	while (!stream.eof()) {
		std::getline(stream, temp);
		x = 0;
		for (auto& item : temp) {
			if (item == 'x') {
				camera.Position.x = x + 0.5f;
				camera.Position.y = y + 0.5f;
				mapUnit[count].Texture = nullptr;
				mapUnit[count].Type = RCMapUnitType::Air;
				mapUnit[count].Passable = true;
			}
			else if (item == 'm') {
				mapUnit[count].Texture = &testDigTexture;
				mapUnit[count].Type = RCMapUnitType::DiagWallRightLeft;
			}
			else if (item == 'd') {
				mapUnit[count].Texture = &testDoorTexture;
				mapUnit[count].Type = RCMapUnitType::Door;
				mapUnit[count].Door = new RCMapDoor(&testDoorTexture);
			}
			else if (item == 'g') {
				mapUnit[count].Texture = &testGlassTexture;
				mapUnit[count].Type = RCMapUnitType::Strip;
				mapUnit[count].Passable = false;
			}
			else if (item == 's') {
				mapUnit[count].Texture = &testStripTexture;
				mapUnit[count].Type = RCMapUnitType::Strip;
				mapUnit[count].Passable = false;
			}
			else if (item != ' ') {
				mapUnit[count].Texture = &testTexture;
				mapUnit[count].Type = RCMapUnitType::Wall;
			}
			else {
				mapUnit[count].Texture = nullptr;
				mapUnit[count].Type = RCMapUnitType::Air;
				mapUnit[count].Passable = true;
			}

			++x;
			++count;
		}
		++y;
	}

	RCTexture spriteTexture(new RCContext(_T("./pics/sprite - copy.png")));
	RCSprite sprite { .texture = &spriteTexture, .x = camera.Position.x - 5, .y = camera.Position.y, .z = 9, .interactable = true,
	                .triggerRange = 1.f,
	.OnTrigger = [&sprite]() {
			sprite.x -= 1.f;
		}};

	RCTexture floorTexture(new RCContext(_T("./pics/tnt_top.png")));
	RCTexture ceilTexture(new RCContext(_T("./pics/greystone.png")));
	RCTexture skyboxTexture(new RCContext(_T("./pics/2k.jpg")));

	RCVideoWindow videoWindow(1920, 1080, _T("RC Engine"));
	auto [renderTarget, context] = videoWindow.GetRenderTuple();
	auto map = new RCMap(mapWidth, mapHeight, (RCMapUnit *) mapUnit);
	RCScene scene(map);

	scene.SetSkyboxRepeat(4);
	scene.EnableSkyBox(true);
	scene.SetFogColor(BLACK);
	scene.SetFogLevel(3.f);
	scene.EnableFog(false);
	scene.SpriteList = new RCSprite*[](&sprite);
	scene.SpriteCount = 1;
	scene.SetFogLevel(5.f);
	scene.SetFloorTexture(&floorTexture);
	scene.SetSkyBoxTexture(&skyboxTexture);
	scene.SetSkyboxRepeat(1);
	scene.SetCeilingTexture(&ceilTexture);

	RCInteractor interactor(&camera, &videoWindow, map, &scene);

	RCRenderer  renderer(renderTarget, &camera, &scene);
	ExMessage   message{};


	renderer.EnableSuperResolution(true);

	videoWindow.SetCursorVisible(false);

	RECT rectangle;
	GetWindowRect(GetHWnd(), &rectangle);

	ClipCursor(&rectangle);

	time_t old = clock();

	int halfWidth = getwidth() / 2;
	int halfHeight = getheight() / 2;

	videoWindow.MoveCursorToCenter();

	float frameRate = 0.01f;
	while (true) {
		frameRate = renderer.Render();
		if (GetForegroundWindow() == GetHWnd()) {
			ExMessage Message{};
			POINT mousePoint;
			GetCursorPos(&mousePoint);
			ScreenToClient(GetHWnd(), &mousePoint);
			auto xDelta = mousePoint.x - halfWidth;
			auto yDelta = mousePoint.y - halfHeight;

			bool flag = false;

			if (xDelta > 1) {
				flag = true;
				Message.x = RCViewLookRight;
			}
			else if (xDelta < -1) {
				flag = true;
				Message.x = RCViewLookLeft;
			}
			if (yDelta > 1) {
				flag = true;
				Message.y = RCViewLookDown;
			}
			else if (yDelta < -1) {
				flag = true;
				Message.y = RCViewLookUp;
			}
			Message.message = RCViewLookChange;

			if (flag) {
				videoWindow.MoveCursorToCenter();
				interactor.ProcessMessage(Message, frameRate, xDelta, yDelta);
			}
		}
		frameRate = frameRate < 0.001f ? 0.001f : frameRate;
		while (videoWindow.Message(&message) && message.message != RCViewLookChange) {
			interactor.ProcessMessage(message, frameRate);
			if (message.vkcode == VK_ESCAPE) {
				exit(0);
			}
		}
		interactor.ProcessDoorAnimation(frameRate);
		interactor.FrameProcess(frameRate);
		interactor.SpriteInteractor();

		renderTarget->Flush();
	}


	return 0;
}