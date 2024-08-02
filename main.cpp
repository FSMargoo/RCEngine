#include <include/RCRenderer.h>
#include <include/RCVideoWindow.h>
#include <include/RCInteractor.h>

#include <fstream>
#include <cmath>
#include <string>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	std::ifstream stream("./res/map.txt");
	int mapWidth;
	int mapHeight;

	RCCamera camera;
	std::string temp;
	std::getline(stream, temp);
	mapWidth = atoi(temp.c_str());
	std::getline(stream, temp);
	mapHeight = atoi(temp.c_str());

	camera.Z = 0;
	camera.SetPitch(0.f);

	RCTexture wallTexture(new RCContext(_T("./res/texture/wall.png")));
	RCTexture digTexture(new RCContext(_T("./res/texture/dig.png")));
	auto doorContext = new RCContext(_T("./res/texture/grate.png"));
	RCTexture testDoorTexture(doorContext);
	auto glassContext = new RCContext(_T("./res/texture/glass.png"));
	RCTexture testGlassTexture(glassContext);
	auto stripContext = new RCContext(_T("./res/texture/strip.png"));
	RCTexture testStripTexture(stripContext);

	auto mapUnit = new RCMapUnit[mapWidth * mapHeight];
	int count = 0;
	int x = 0;
	int y = 0;
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
				mapUnit[count].Texture = &digTexture;
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
				mapUnit[count].Texture = &wallTexture;
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

	RCTexture spriteTexture(new RCContext(_T("./res/texture/sprite.png")));
	RCSprite sprite { .texture = &spriteTexture, .x = camera.Position.x - 5, .y = camera.Position.y, .z = 9, .interactable = true,
	                .triggerRange = 1.f,
	.OnTrigger = [&sprite]() {
			sprite.x -= 1.f;
		}};

	RCTexture floorTexture(new RCContext(_T("./res/texture/floor.png")));
	RCTexture ceilingTexture(new RCContext(_T("./res/texture/ceiling.png")));
	RCTexture skyboxTexture(new RCContext(_T("./res/texture/skybox.jpg")));

	RCVideoWindow videoWindow(640, 480, _T("RC Engine Demo"));
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
	scene.SetCeilingTexture(&ceilingTexture);

	RCInteractor interactor(&camera, &videoWindow, map, &scene);

	RCRenderer  renderer(renderTarget, &camera, &scene);
	ExMessage   message{};

	renderer.EnableSuperResolution(false);

	videoWindow.SetCursorVisible(false);

	RECT rectangle;
	GetWindowRect(GetHWnd(), &rectangle);

	ClipCursor(&rectangle);
	
	videoWindow.MoveCursorToCenter();

	float frameRate = 0.01f;

	setbkmode(TRANSPARENT);

	while (true) {
		frameRate = renderer.Render();
		interactor.Interact(frameRate);
		settextcolor(BLACK);
		outtextxy(21, 21, _T("操作说明："));
		outtextxy(21, 38, _T("   按下 'F' 开门"));
		outtextxy(21, 51, _T("   按下 'ESC' 退出"));
		outtextxy(21, 64, _T("   'W' 'S' 'A' 'D' 左右移动"));
		outtextxy(21, 77, _T("   'ctrl' 潜行 'shift' 疾跑"));
		settextcolor(WHITE);
		outtextxy(20, 20, _T("操作说明："));
		outtextxy(20, 36, _T("   按下 'F' 开门"));
		outtextxy(20, 50, _T("   按下 'ESC' 退出"));
		outtextxy(20, 63, _T("   'W' 'S' 'A' 'D' 左右移动"));
		outtextxy(20, 76, _T("   'ctrl' 潜行 'shift' 疾跑"));
		renderTarget->Flush();
	}


	return 0;
}