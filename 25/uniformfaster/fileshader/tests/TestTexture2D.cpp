#include "TestTexture2D.h"



#include "../Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestTexture2D::TestTexture2D() :
        m_TranslationA(200.0f, 200.0f, 0.0f),
        m_TranslationB(400.0f, 200.0f, 0.0f),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
	{
        float positions[] = {
            -50.0f,   -50.0f, 0.0f, 0.0f,
             50.0f,   -50.0f, 1.0f, 0.0f,
             50.0f,    50.0f, 1.0f, 1.0f,
            -50.0f,    50.0f, 0.0f, 1.0f,
        };
        unsigned int indices[] = {
            0,1,2,
            2,3,0,
        };
        assert(sizeof(positions) == (_countof(positions) * sizeof(float)));

        //CALLGL(glDisable(GL_BLEND));
        //CALLGL(glBlendFunc(GL_ONE, GL_ZERO));

        CALLGL(glEnable(GL_BLEND));
        CALLGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, _countof(positions) * sizeof(float));
        


        VertexBufferLayout layout;
        layout.Push<float>(2); // a vertex has 2 points
        layout.Push<float>(2); // a vertex has 2 points
        m_VAO->AddBuffer(*m_VertexBuffer, layout);



        // index buffer object
        // IndexBuffer ib(indices, _countof(indices));
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, _countof(indices));

        // Shader
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


        m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
        int slot = 0;
        m_Shader->SetUniform1i("u_Texture", slot);
    }
	TestTexture2D::~TestTexture2D()
	{}

	void TestTexture2D::OnUpdate(float deltatime)
	{}
	void TestTexture2D::OnRender()
	{
		CALLGL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		CALLGL(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        m_Texture->Bind();

        {
            // 200 left, 200 up
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        {
            // 400 left, 200 up
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}
    void TestTexture2D::OnImGuiRender()
    {

        static float f = 0.0f;
        static int counter = 0;
        ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    }
}