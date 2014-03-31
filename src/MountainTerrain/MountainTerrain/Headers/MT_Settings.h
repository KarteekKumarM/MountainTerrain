//---------------------------------------------------------------------------------------------------
//-----------------------------------MOUNTAIN TERRAIN SETTINGS---------------------------------------
//---------------------------------------------------------------------------------------------------

// Rendering Settings
#define SCREEN_SHOT_PATH "../../../pics/"
#define ANTIALIASING_ENABLED true;
#define WIRE_MESH false;

// Terrain Resources
#define HEIGHTMAP_PATH "Resources/HeightMapImage.bmp"
#define TEXTURE_GRASS_PATH "Resources/texture_grass.bmp"
#define TEXTURE_ROCK_PATH "Resources/texture_rock.bmp"
#define TEXTURE_WATER_PATH "Resources/texture_water.bmp"
#define VERTEX_SHADER_PATH L"VertexShader.cso"
#define PIXEL_SHADER_PATH L"PixelShader.cso"

// Terrain Settings
#define GRID_CELL_WIDTH 0.4f
#define GRID_CELL_DEPTH 0.4f
#define SEA_LEVEL 1.0f

// Lighting Settings
#define LIGHT_TOGGLE true
#define TEXTURE_TOGGLE true
#define LIGHT_AMBIENT_INTENSITY XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
#define LIGHT_DIFFUSE_DIRECTION XMFLOAT3(0.03f, 0.03f, -0.75f);
/*
Warm_Fluroscent = XMFLOAT4(1.0f, 0.96f, 0.98f, 1.0f);
High_Pressure_Sodium = XMFLOAT4(1.0f, 0.72f, 0.3f, 1.0f);
White = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
*/
#define LIGHT_DIFFUSE_INTENSITY XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)

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

// Camera
#define MATH_DEGREE_TO_RADIAN 0.0174532925f
#define ANGLE_TURN_PER_UNIT_TIME 10
#define DISTANCE_MOVE_PER_UNIT_TIME 1.0f
#define CAMERA_EYE 0.0f, 20.0f, 0.0f, 0.0f
#define CAMERA_LOOKAT 0.0f, 0.0f, 0.0f, 0.0f
#define CAMERA_UP 0.0f, 0.0f, 1.0f, 0.0f

// some defaults
#define XMFLOAT3_UP XMFLOAT3(0, 1, 0)