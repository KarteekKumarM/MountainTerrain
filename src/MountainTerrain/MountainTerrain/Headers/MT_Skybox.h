#include <d3d11.h>
#include <DirectXMath.h>

#include "MT_Shader.h"
#include "MT_Texture.h"

using namespace DirectX;

struct SkyboxVertex
{
	XMFLOAT3 Position;
	XMFLOAT3 TextureCoord;
};

enum SkyboxFace
{
	SKYBOX_FACE_TOP,
	SKYBOX_FACE_BOTTOM,
	SKYBOX_FACE_LEFT,
	SKYBOX_FACE_RIGHT,
	SKYBOX_FACE_FRONT,
	SKYBOX_FACE_BACK,
	SKYBOX_FACE_COUNT
};

enum SkyboxVertPosition
{
	SKYBOX_VERT_UL,
	SKYBOX_VERT_UR,
	SKYBOX_VERT_LL,
	SKYBOX_VERT_LR,
	SKYBOX_VERT_COUNT
};

// two triangles per face
#define SKYBOX_NUM_INDICES SKYBOX_FACE_COUNT * 6
#define SKYBOX_NUM_VERTICES SKYBOX_FACE_COUNT * SKYBOX_VERT_COUNT

class MT_Skybox
{
private:
	XMFLOAT3 m_verticesRelativeToOrigin[SKYBOX_FACE_COUNT][SKYBOX_VERT_COUNT];
	SkyboxVertex m_verticesRelativeToPlayer[SKYBOX_NUM_VERTICES];
	WORD m_indices[SKYBOX_NUM_INDICES];

	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;

	MT_Shader* m_shader;
	MT_Texture* m_texture;

	void InitVertices();
	void InitIndices();
	void LoadVertexBuffer(ID3D11Device *d3dDevice);
	void LoadIndexBuffer(ID3D11Device *d3dDevice);
	void CleanVertexBuffer();
public:
	void Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void Update(XMFLOAT3 playerPosition);
	void SetShadersActive(ID3D11DeviceContext *d3dDeviceContext);
	void RenderFrame(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void Clean();
};