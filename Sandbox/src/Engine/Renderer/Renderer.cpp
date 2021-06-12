#include "xpch.h"
#include "GL/glew.h"
#include "Renderer.h"
#include "Camera.h"
#include "Engine/Core/Application.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Engine/Renderer/ShaderProgram.h"
#include <Engine/ECS/Components.h>
#include "Engine/Renderer/VertexBufferLayout.h"
#include "Engine/Utilities/Vec.h"
#include "Engine/Debug/Profiler.h"

/*
* TO DO: STOP USING GEOMETRY SHADER FOR PARTICLES
*/

const char* vertex_shader = R"(#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordinate;
layout(location = 2) in vec4 colorCoordinate;
layout(location = 3) in float textureIndex;

uniform mat4 u_MVP;

out vec4 v_color;
out float v_textureIndex;
out vec2 v_textureCoordinate;

void main()
{
	gl_Position = u_MVP * vec4(vertexPosition,1.0);
	v_color = colorCoordinate;
	v_textureIndex = textureIndex;
	v_textureCoordinate = textureCoordinate;
}
)";

const char* vetex_particle_shader = R"(#version 330 core
layout (location = 0) in vec3 vertexPosition;
uniform mat4 ModelViewMatrix;

void main()
{
	gl_Position = ModelViewMatrix * vec4(vertexPosition,1.0);

}
	)";

const char* fragment_particle_shader = R"(#version 330 core
layout (location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0);
}
	)";

const char* geometry_particle_shader = R"(#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float Size2; //Half the width of the quad
uniform mat4 ProjectionMatrix;

void main()
{
	mat4 m = ProjectionMatrix;

	gl_Position = m * (vec4(-Size2,-Size2,0.0,0.0) + 
					gl_in[0].gl_Position);
	EmitVertex();


	gl_Position = m * (vec4(Size2,-Size2,0.0,0.0) + 
					gl_in[0].gl_Position);
	EmitVertex();


	gl_Position = m * (vec4(-Size2,Size2,0.0,0.0) + 
					gl_in[0].gl_Position);
	EmitVertex();







	gl_Position = m * (vec4(Size2,Size2,0.0,0.0) + 
					gl_in[0].gl_Position);
	EmitVertex();

	EndPrimitive();
}
	)";


const char* fragment_shader =
R"(#version 330 core
layout(location = 0) out vec4 FragColor;

in vec4 v_color;
in vec2 v_textureCoordinate;
in float v_textureIndex;

uniform sampler2D u_textures[32];
void main()
{
	switch(int(v_textureIndex)) {
	case 0:
		FragColor = v_color;
		break;
	case 1:
		FragColor = texture(u_textures[1],v_textureCoordinate) * v_color;
		break;
	case 2:
		FragColor = texture(u_textures[2],v_textureCoordinate)* v_color;
		break;
	case 3:
		FragColor = texture(u_textures[3],v_textureCoordinate)* v_color;
		break;
	case 4:
		FragColor = texture(u_textures[4],v_textureCoordinate)* v_color;
		break;
	case 5:
		FragColor = texture(u_textures[5],v_textureCoordinate)* v_color;
		break;
	case 6:
		FragColor = texture(u_textures[6],v_textureCoordinate)* v_color;
		break;
	case 7:
		FragColor = texture(u_textures[7],v_textureCoordinate)* v_color;
		break;
	case 8:
		FragColor = texture(u_textures[8],v_textureCoordinate)* v_color;
		break;
	case 9:
		FragColor = texture(u_textures[9],v_textureCoordinate)* v_color;
		break;
	case 10:
		FragColor = texture(u_textures[10],v_textureCoordinate)* v_color;
		break;
	case 11:
		FragColor = texture(u_textures[11],v_textureCoordinate)* v_color;
		break;
	case 12:
		FragColor = texture(u_textures[12],v_textureCoordinate)* v_color;
		break;
	case 13:
		FragColor = texture(u_textures[13],v_textureCoordinate)* v_color;
		break;
	case 14:
		FragColor = texture(u_textures[14],v_textureCoordinate)* v_color;
		break;
	case 15:
		FragColor = texture(u_textures[15],v_textureCoordinate)* v_color;
		break;
	case 16:
		FragColor = texture(u_textures[16],v_textureCoordinate)* v_color;
		break;
	case 17:
		FragColor = texture(u_textures[17],v_textureCoordinate)* v_color;
		break;
	case 18:
		FragColor = texture(u_textures[18],v_textureCoordinate)* v_color;
		break;
	case 19:
		FragColor = texture(u_textures[19],v_textureCoordinate)* v_color;
		break;
	case 20:
		FragColor = texture(u_textures[20],v_textureCoordinate)* v_color;
		break;
	case 21:
		FragColor = texture(u_textures[21],v_textureCoordinate)* v_color;
		break;
	case 22:
		FragColor = texture(u_textures[22],v_textureCoordinate)* v_color;
		break;
	case 23:
		FragColor = texture(u_textures[23],v_textureCoordinate)* v_color;
		break;
	case 24:
		FragColor = texture(u_textures[24],v_textureCoordinate)* v_color;
		break;
	case 25:
		FragColor = texture(u_textures[25],v_textureCoordinate)* v_color;
		break;
	case 26:
		FragColor = texture(u_textures[26],v_textureCoordinate)* v_color;
		break;
	case 27:
		FragColor = texture(u_textures[27],v_textureCoordinate)* v_color;
		break;
	case 28:
		FragColor = texture(u_textures[28],v_textureCoordinate)* v_color;
		break;
	case 29:
		FragColor = texture(u_textures[29],v_textureCoordinate)* v_color;
		break;
	case 30:
		FragColor = texture(u_textures[30],v_textureCoordinate)* v_color;
		break;
	case 31:
		FragColor = texture(u_textures[31],v_textureCoordinate)* v_color;
		break;

	}	
})";





struct RendererVertex {
	

	Vec2<float> position;
	Vec2<float> textureCoord;
	Vec4<float> colorCord;
	Vec1<float> textureIndex;

	static VertexBufferLayout getLayout() {
		return {{2, ShaderBool::False, DataType::Float },
			    {2, ShaderBool::False, DataType::Float},
			    {4, ShaderBool::False, DataType::Float },
			    {1, ShaderBool::False, DataType::Float }};
	}
};


struct ParticleVertex {
	glm::vec2 position;
	

	static VertexBufferLayout getLayout() {
		return {{2,ShaderBool::False,DataType::Float}};
	}
};


glm::vec4 Renderer::s_clear_color = glm::vec4(0.1, 0.1, 0.1, 1.0);





static int m_index_IndexBuffer = 0;
static int m_index_VertexBuffer = 0;

static unsigned int* m_texture_array = nullptr;

static int textureSlots = 0;

VertexBuffer<RendererVertex>* vertexBuffer = nullptr;
IndexBuffer* indexBuffer = nullptr;
VertexArray* vao = nullptr;
static ShaderProgram* shader_program = nullptr;


/////////////////////////////////////////////// PARTICLE STUFF
static ShaderProgram* particle_shader_program = nullptr;
static VertexBuffer<ParticleVertex>* particle_vertex_buffer = nullptr;
static VertexArray* particle_vertex_array = nullptr;





static int DrawCallsPerFrame = 0;

static void RenderGeometry() { 

	xENGINE_PROFILE_FUNCTION();
	
	vertexBuffer->bindData();
	indexBuffer->bindData();
	vao->bind();

	shader_program->setUniformMat4f("u_MVP", application::getActiveScene().getCameraController()->getCamera()->getViewProjMat());

	
	glDrawElements(GL_TRIANGLES, m_index_IndexBuffer, GL_UNSIGNED_INT, nullptr);


	m_index_IndexBuffer = 0;
	m_index_VertexBuffer = 0;

	for (int i = 0; i < textureSlots - 1; i++)
	{
		m_texture_array[i] = 0;
	}

	DrawCallsPerFrame++;

}



void Renderer::Init()

{
	xENGINE_PROFILE_FUNCTION();
	textureSlots = Texture::numberTextureSlots();

	Shader VertexShader = Shader(Shader::ShaderType::vertex);
	VertexShader.CompileString(std::string(vertex_shader));

	Shader FragmentShader = Shader(Shader::ShaderType::fragment);
	FragmentShader.CompileString(std::string(fragment_shader));

	shader_program = new ShaderProgram(VertexShader, FragmentShader);


	//------------------
	//Shader VertexParticleShader(Shader::ShaderType::vertex);
	//VertexParticleShader.CompileString(std::string(vetex_particle_shader));

	//Shader GeometryParticleShader(Shader::ShaderType::geometry);
	//GeometryParticleShader.CompileString(std::string(geometry_particle_shader));

	//Shader FragmentParticleShader(Shader::ShaderType::fragment);
	//FragmentParticleShader.CompileString(std::string(fragment_particle_shader));

	//particle_shader_program = new ShaderProgram(VertexParticleShader, GeometryParticleShader, FragmentParticleShader);

	
	//particle_vertex_buffer = new VertexBuffer<ParticleVertex>(xEngine::Component::ParticleSystem::MAX_PARTICLE_COUNT);
	//particle_vertex_array = new VertexArray();
	//particle_vertex_array->setLayout(*particle_vertex_buffer, ParticleVertex::getLayout());

	//------------------------------------------------------
	
	m_texture_array = new unsigned int[Texture::numberTextureSlots() - 1]; //reserve the first texture slot for the white texture
	for (int i = 0; i < textureSlots - 1; i++)
	{
		m_texture_array[0] = 0;
	}


	int* uniformNumbers = new int[textureSlots];
	for (int i = 0; i < textureSlots; i++)
		uniformNumbers[i] = i;

	shader_program->setUniform1iv("u_textures",textureSlots, uniformNumbers);
	delete[] uniformNumbers;


	vertexBuffer = new VertexBuffer<RendererVertex>(Renderer::MAX_VERTEX_LENGTH);
	vao = new VertexArray();
	vao->setLayout(*vertexBuffer, RendererVertex::getLayout());
	indexBuffer = new  IndexBuffer(Renderer::MAX_INDEX_LENGTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vertexBuffer->bind();
	vao->bind();
	indexBuffer->bind();
	shader_program->bind();
}

void Renderer::Shutdown()
{
	delete particle_shader_program;
	particle_shader_program = nullptr;
	delete shader_program;
	delete[] m_texture_array;
	m_texture_array = nullptr;
	shader_program = nullptr;

	delete vertexBuffer;
	delete indexBuffer;
	delete vao;

	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	vao = nullptr;
}



void Renderer::DrawQuad(const Vec2<float>& center, const Vec2<float>& dimensions, float angle, Vec4<float> color)
{
	xENGINE_PROFILE_FUNCTION();
	
	if (m_index_IndexBuffer + 6 >= MAX_INDEX_LENGTH || m_index_VertexBuffer + 4 >= MAX_VERTEX_LENGTH)
		RenderGeometry();

	(*indexBuffer)[m_index_IndexBuffer + 0] = m_index_VertexBuffer + 0;
	(*indexBuffer)[m_index_IndexBuffer + 1] = m_index_VertexBuffer + 3;
	(*indexBuffer)[m_index_IndexBuffer + 2] = m_index_VertexBuffer + 1;
	(*indexBuffer)[m_index_IndexBuffer + 3] = m_index_VertexBuffer + 1;
	(*indexBuffer)[m_index_IndexBuffer + 4] = m_index_VertexBuffer + 2;
	(*indexBuffer)[m_index_IndexBuffer + 5] = m_index_VertexBuffer + 3;

	(*vertexBuffer)[m_index_VertexBuffer + 0] = { {center.x - dimensions.x / 2.0f, center.y + dimensions.y / 2.0f},{0.0f, 1.0f},color,{0.0f  } }; //top left
	(*vertexBuffer)[m_index_VertexBuffer + 1] = { {center.x + dimensions.x / 2.0f, center.y + dimensions.y / 2.0f},{1.0f, 1.0f},color,{0.0f } }; //top right
	(*vertexBuffer)[m_index_VertexBuffer + 2] = { {center.x + dimensions.x / 2.0f, center.y - dimensions.y / 2.0f},{1.0f, 0.0f},color,{0.0f } }; //bottom right
	(*vertexBuffer)[m_index_VertexBuffer + 3] = { {center.x - dimensions.x / 2.0f,center.y - dimensions.y / 2.0f},{0.0f, 0.0f},color,{0.0f} }; //bottom left

	if (angle == 0.0f)
	{
		m_index_IndexBuffer += 6;
		m_index_VertexBuffer += 4;
		return;
	}
	float center_x = (float)((*vertexBuffer)[m_index_VertexBuffer + 1].position.x + (*vertexBuffer)[m_index_VertexBuffer + 0].position.x) / (float)2.0f;
	float center_y = (float)((*vertexBuffer)[m_index_VertexBuffer + 0].position.y + (*vertexBuffer)[m_index_VertexBuffer + 3].position.y) / (float)2.0f;

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.x -= center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.x -= center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.x -= center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.x -= center_x;

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.y -= center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.y -= center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.y -= center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.y -= center_y;


	float original_x[4];
	original_x[0] = (*vertexBuffer)[m_index_VertexBuffer + 0].position.x;
	original_x[1] = (*vertexBuffer)[m_index_VertexBuffer + 1].position.x;
	original_x[2] = (*vertexBuffer)[m_index_VertexBuffer + 2].position.x;
	original_x[3] = (*vertexBuffer)[m_index_VertexBuffer + 3].position.x;
	angle = angle * (glm::pi<float>() / 180.0f);


	(*vertexBuffer)[m_index_VertexBuffer + 0].position.x = (*vertexBuffer)[m_index_VertexBuffer + 0].position.x * cos(angle) - (*vertexBuffer)[m_index_VertexBuffer + 0].position.y * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.x = (*vertexBuffer)[m_index_VertexBuffer + 1].position.x * cos(angle) - (*vertexBuffer)[m_index_VertexBuffer + 1].position.y * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.x = (*vertexBuffer)[m_index_VertexBuffer + 2].position.x * cos(angle) - (*vertexBuffer)[m_index_VertexBuffer + 2].position.y * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.x = (*vertexBuffer)[m_index_VertexBuffer + 3].position.x * cos(angle) - (*vertexBuffer)[m_index_VertexBuffer + 3].position.y * sin(angle);

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.y = (*vertexBuffer)[m_index_VertexBuffer + 0].position.y * cos(angle) + original_x[0] * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.y = (*vertexBuffer)[m_index_VertexBuffer + 1].position.y * cos(angle) + original_x[1] * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.y = (*vertexBuffer)[m_index_VertexBuffer + 2].position.y * cos(angle) + original_x[2] * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.y = (*vertexBuffer)[m_index_VertexBuffer + 3].position.y * cos(angle) + original_x[3] * sin(angle);



	(*vertexBuffer)[m_index_VertexBuffer + 0].position.y += center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.y += center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.y += center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.y += center_y;

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.x += center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.x += center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.x += center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.x += center_x;



	m_index_IndexBuffer += 6;
	m_index_VertexBuffer += 4;
}

void Renderer::DrawQuad(const Vec2<float>& center, const Vec2<float>& dimensions, float angle, SpriteTexture* texture, Vec4<float> color)
{
	xENGINE_PROFILE_FUNCTION();
	//is there space in the index and vertex arrays
	if (m_index_IndexBuffer + 6 >= MAX_INDEX_LENGTH || m_index_VertexBuffer + 4 >= MAX_VERTEX_LENGTH)
		RenderGeometry();

	int currentIndex = -1;
	for (int i = 0; i < textureSlots - 1; i++)
	{
		if (m_texture_array[i] == 0) { //there is space
			currentIndex = i;
			texture->getbaseTexture()->bind((unsigned int)i + 1);
			//texture->bind((unsigned int)i+1);
			break;
		}
		if (m_texture_array[i] == texture->getbaseTexture()->getId())
		{
			currentIndex = i; //the texture slot where the same texture is bound to
			break;
		}
	}

	if (currentIndex == -1)
	{
		RenderGeometry();
		currentIndex = 0;
		//texture->bind(0);
		texture->getbaseTexture()->bind(0);
	}
		
	//m_texture_array[currentIndex] = texture->getId();
	m_texture_array[currentIndex] = texture->getbaseTexture()->getId();

	//ready to draw
	(*indexBuffer)[m_index_IndexBuffer + 0] = m_index_VertexBuffer + 0;
	(*indexBuffer)[m_index_IndexBuffer + 1] = m_index_VertexBuffer + 3;
	(*indexBuffer)[m_index_IndexBuffer + 2] = m_index_VertexBuffer + 1;
	(*indexBuffer)[m_index_IndexBuffer + 3] = m_index_VertexBuffer + 1;
	(*indexBuffer)[m_index_IndexBuffer + 4] = m_index_VertexBuffer + 2;
	(*indexBuffer)[m_index_IndexBuffer + 5] = m_index_VertexBuffer + 3;

	glm::vec2 bottomLeft = texture->getTextureCords()[0];
	glm::vec2 topRight = texture->getTextureCords()[1];
	glm::vec2 bottomRight = {topRight.x,bottomLeft.y};
	glm::vec2 topLeft = {bottomLeft.x, topRight.y};
	

	
	(*vertexBuffer)[m_index_VertexBuffer + 0] = { {center.x - dimensions.x / 2.0f, center.y + dimensions.y / 2.0f},{topLeft.x, topLeft.y},color,{(float)currentIndex + 1 } }; //top left
	(*vertexBuffer)[m_index_VertexBuffer + 1] = { {center.x + dimensions.x / 2.0f, center.y + dimensions.y / 2.0f},{topRight.x, topRight.y},color,{(float)currentIndex + 1} }; //top right
	(*vertexBuffer)[m_index_VertexBuffer + 2] = { {center.x + dimensions.x / 2.0f, center.y - dimensions.y / 2.0f},{bottomRight.x, bottomRight.y},color,{(float)currentIndex + 1} }; //bottom right
	(*vertexBuffer)[m_index_VertexBuffer + 3] = { {center.x - dimensions.x / 2.0f,center.y - dimensions.y / 2.0f},{bottomLeft.x, bottomLeft.y},color,{(float)currentIndex + 1} }; //bottom left

	if (angle == 0.0f)
	{
		m_index_IndexBuffer += 6;
		m_index_VertexBuffer += 4;
		return;
	}
	float center_x = (float)((*vertexBuffer)[m_index_VertexBuffer + 1].position.x + (*vertexBuffer)[m_index_VertexBuffer + 0].position.x) / (float)2.0f;
	float center_y = (float)((*vertexBuffer)[m_index_VertexBuffer + 0].position.y + (*vertexBuffer)[m_index_VertexBuffer + 3].position.y) / (float)2.0f;

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.x -= center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.x -= center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.x -= center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.x -= center_x;

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.y -= center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.y -= center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.y -= center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.y -= center_y;


	float original_x[4];
	original_x[0] = (*vertexBuffer)[m_index_VertexBuffer + 0].position.x;
	original_x[1] = (*vertexBuffer)[m_index_VertexBuffer + 1].position.x;
	original_x[2] = (*vertexBuffer)[m_index_VertexBuffer + 2].position.x;
	original_x[3] = (*vertexBuffer)[m_index_VertexBuffer + 3].position.x;

	angle = angle * (glm::pi<float>() / 180.0f);

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.x = (*vertexBuffer)[m_index_VertexBuffer + 0].position.x * cos(angle) - (*vertexBuffer)[m_index_VertexBuffer + 0].position.y * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.x = (*vertexBuffer)[m_index_VertexBuffer + 1].position.x * cos(angle) - (*vertexBuffer)[m_index_VertexBuffer + 1].position.y * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.x = (*vertexBuffer)[m_index_VertexBuffer + 2].position.x * cos(angle) - (*vertexBuffer)[m_index_VertexBuffer + 2].position.y * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.x = (*vertexBuffer)[m_index_VertexBuffer + 3].position.x * cos(angle) - (*vertexBuffer)[m_index_VertexBuffer + 3].position.y * sin(angle);

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.y = (*vertexBuffer)[m_index_VertexBuffer + 0].position.y * cos(angle) + original_x[0] * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.y = (*vertexBuffer)[m_index_VertexBuffer + 1].position.y * cos(angle) + original_x[1] * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.y = (*vertexBuffer)[m_index_VertexBuffer + 2].position.y * cos(angle) + original_x[2] * sin(angle);
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.y = (*vertexBuffer)[m_index_VertexBuffer + 3].position.y * cos(angle) + original_x[3] * sin(angle);



	(*vertexBuffer)[m_index_VertexBuffer + 0].position.y += center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.y += center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.y += center_y;
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.y += center_y;

	(*vertexBuffer)[m_index_VertexBuffer + 0].position.x += center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 1].position.x += center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 2].position.x += center_x;
	(*vertexBuffer)[m_index_VertexBuffer + 3].position.x += center_x;

	m_index_IndexBuffer += 6;
	m_index_VertexBuffer += 4;
}

struct particleVertex {
	glm::vec2 position;

	static VertexBufferLayout getLayout() {
		return { {2, ShaderBool::False, DataType::Float } };
	}
};

void Renderer::DrawParticleSystem(xEngine::Component::ParticleSystem& ps)
{
	for (int i = 0; i < ps.maxParticles; i++)
	{
		if (ps.particles[i].alive)
		{
			Renderer::DrawQuad({ ps.particles[i].position.x,ps.particles[i].position.y }, *(Vec2<float>*) & (ps.particles[i].currentSize), ps.particles[i].angle, *(Vec4<float>*) & (ps.particles[i].currentColor));
		}
	}
}

void Renderer::DrawEntity(xEngine::Component::Sprite2DComponent& sprite, xEngine::Component::TransformComponent& transform)
{
	
	if(sprite.texture)
		Renderer::DrawQuad({transform.x, transform.y}, {transform.x_scale, transform.y_scale}, transform.rot, sprite.texture, sprite.color);
	else
		Renderer::DrawQuad({ transform.x, transform.y }, { transform.x_scale, transform.y_scale } , transform.rot, sprite.color);
}


glm::vec4 Renderer::GetClearColor()
{
	return s_clear_color;
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	s_clear_color = color;
	glClearColor(s_clear_color.x, s_clear_color.y, s_clear_color.z, s_clear_color.w);
}

void Renderer::Clear()
{
	glClearColor(s_clear_color.x, s_clear_color.y, s_clear_color.z, s_clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::StartFrame()
{
	DrawCallsPerFrame = 0;
	RenderGeometry();
}

void Renderer::EndFrame()
{
	DrawCallsPerFrame = 0;
	RenderGeometry(); 
}

int Renderer::getDrawCallsPerFrame()
{
	return DrawCallsPerFrame;
}

void Renderer::SetModeParticle()
{
	//might keep empty
}

void Renderer::SetModeNormal()
{
	//might keep empty
}

ShaderProgram* Renderer::getParticleProgram()
{
	return particle_shader_program;
}
