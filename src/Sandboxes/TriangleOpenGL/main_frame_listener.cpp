#include "main_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"

#include <glad/glad.h>


namespace Kmplete
{
    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _vao(0)
        , _vbo(0)
        , _ebo(0)
        , _shaderProgramId(0)
        , _windowFramebufferResizeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowFramebufferResizeEvent))
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    MainFrameListener::~MainFrameListener()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Initialize()
    {
        const char* vertexShaderSource = "#version 450 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 450 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n\0";

        const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        int success = 0;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            KMP_LOG_ERROR("vertex shader compilation failed: {}", infoLog);
        }

        const auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            KMP_LOG_ERROR("fragment shader compilation failed: {}", infoLog);
        }

        _shaderProgramId = glCreateProgram();
        glAttachShader(_shaderProgramId, vertexShader);
        glAttachShader(_shaderProgramId, fragmentShader);
        glLinkProgram(_shaderProgramId);
        glGetProgramiv(_shaderProgramId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(_shaderProgramId, 512, nullptr, infoLog);
            KMP_LOG_ERROR("shader program linking failed: {}", infoLog);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left  
             0.5f, -0.5f, 0.0f, // right 
             0.0f,  0.5f, 0.0f  // top   
        };

        unsigned int indices[] = {
            0, 1, 2
        };

        glCreateVertexArrays(1, &_vao);
        glCreateBuffers(1, &_vbo);
        glCreateBuffers(1, &_ebo);
        glCreateBuffers(1, &_ebo);

        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vao);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glViewport(0, 0, _mainWindow.GetWindowedSize().x, _mainWindow.GetWindowedSize().y);
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Finalize()
    {
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);
        glDeleteVertexArrays(1, &_vao);
        glDeleteProgram(_shaderProgramId);
    }
    //--------------------------------------------------------------------------

    bool MainFrameListener::OnWindowFramebufferResizeEvent(Events::WindowFramebufferResizeEvent& event)
    {
        glViewport(0, 0, event.GetWidth(), event.GetHeight());
        return true;
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {}
    //--------------------------------------------------------------------------

    void MainFrameListener::Render()
    {
        glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(_shaderProgramId);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    }
    //--------------------------------------------------------------------------
}