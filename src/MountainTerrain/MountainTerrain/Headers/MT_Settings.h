//---------------------------------------------------------------------------------------------------
//-----------------------------------MOUNTAIN TERRAIN SETTINGS---------------------------------------
//---------------------------------------------------------------------------------------------------

// Window
#define WINDOW_NAME L"MountainTerrainWindowClass"
#define WINDOW_TITLE L"Mountain Terran - By Karteek Mekala"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define WINDOW_POS_X 300
#define WINDOW_POS_Y 300
#define WINDOW_TOGGLE_FULLSCREEN false

//// Stats Window
#define STATS_WINDOW_NAME L"MountainTerrainStatsWindowClass"
#define STATS_WINDOW_TITLE L"Statistics"
#define STATS_WINDOW_WIDTH 400
#define STATS_WINDOW_HEIGHT 300
#define STATS_WINDOW_POS_X (WINDOW_POS_X + WINDOW_WIDTH + 50)
#define STATS_WINDOW_POS_Y WINDOW_POS_Y

// Rendering Settings
#define SCREEN_SHOT_PATH "../../../pics/"
#define ANTIALIASING_ENABLED true;
#define WIRE_MESH false;

// Terrain Resources
#define TERRAIN_HEIGHTMAP_PATH "Resources/HeightMapImage.bmp"
#define TERRAIN_TEXTURE_GRASS_PATH "Resources/texture_grass.bmp"
#define TERRAIN_TEXTURE_ROCK_PATH "Resources/texture_rock.bmp"
#define TERRAIN_TEXTURE_WATER_PATH "Resources/texture_water.bmp"
#define TERRAIN_VERTEX_SHADER_PATH L"MT_Terrain_VertexShader.cso"
#define TERRAIN_PIXEL_SHADER_PATH L"MT_Terrain_PixelShader.cso"

#define SKYBOX_TEXTURE_TOP "Resources/FullMoonUp2048.png"
#define SKYBOX_TEXTURE_BOTTOM "Resources/FullMoonDown2048.png"
#define SKYBOX_TEXTURE_BACK "Resources/FullMoonBack2048.png"
#define SKYBOX_TEXTURE_FRONT "Resources/FullMoonFront2048.png"
#define SKYBOX_TEXTURE_RIGHT "Resources/FullMoonRight2048.png"
#define SKYBOX_TEXTURE_LEFT "Resources/FullMoonLeft2048.png"

// Skybox Resources
#define SKYBOX_VERTEX_SHADER_PATH L"MT_Skybox_VertexShader.cso"
#define SKYBOX_PIXEL_SHADER_PATH L"MT_Skybox_PixelShader.cso"

// Terrain Settings
#define GRID_CELL_WIDTH 1.5f
#define GRID_CELL_DEPTH 1.5f
#define SEA_LEVEL 1.0f
#define TOGGLE_FLAT_GRID false

// Lighting Settings
#define LIGHT_TOGGLE true
#define TEXTURE_TOGGLE true
#define LIGHT_AMBIENT_INTENSITY XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
#define LIGHT_DIFFUSE_DIRECTION XMFLOAT3(0.75f, 0.75f, 0.75f);
/*
Warm_Fluroscent = XMFLOAT4(1.0f, 0.96f, 0.98f, 1.0f);
High_Pressure_Sodium = XMFLOAT4(1.0f, 0.72f, 0.3f, 1.0f);
White = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
*/
#define LIGHT_DIFFUSE_INTENSITY XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)

// Input Keys
#define KEY_FORWARD 'w'
#define KEY_BACK 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_UP 'e'
#define KEY_DOWN 'c'
#define KEY_SCREEN_SHOT 'k'
#define KEY_TOGGLE_LIGHT 'l'
#define KEY_TOGGLE_TEXTURE 't'
#define KEY_TOGGLE_WIREMESH 'm'
#define KEY_TOGGLE_SKYBOX '2'
#define KEY_TOGGLE_TERRAIN '1'

// Camera
#define MATH_DEGREE_TO_RADIAN 0.0174532925f
#define ANGLE_TURN_PER_UNIT_TIME 10
#define DISTANCE_MOVE_PER_UNIT_TIME 2.5f
#define CAMERA_EYE 0.0f, 20.0f, 0.0f, 0.0f
#define CAMERA_LOOKAT 0.0f, 0.0f, 0.0f, 0.0f
#define CAMERA_UP 0.0f, 0.0f, 1.0f, 0.0f

// skybox
#define SKYBOX_WIDTH 1000
#define SKYBOX_HEIGHT 1000
#define SKYBOX_DEPTH 1000

// some defaults
#define XMFLOAT3_UP XMFLOAT3(0, 1, 0)