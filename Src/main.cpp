#include "glad.h"
#include "glfw3.h"
#include "iostream"
#include "sstream"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

float SCREEN_WIDTH = 1280.0f;
float SCREEN_HRIGHT = 720.0f;

float delta_time = 0.0f;
float last_frame = 0.0f;

float last_x = SCREEN_WIDTH / 2.0f, last_y = SCREEN_HRIGHT / 2.0f;
unsigned int light_bits = 0;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

void ProcessInput(GLFWwindow *window)
{
    float speed = camera.mouse_speed * delta_time;
    // 移动
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Move(camera.forward * speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Move(-camera.forward * speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Move(-camera.right * speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Move(camera.right * speed);
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true); break;
        case GLFW_KEY_1: light_bits ^= 1; break;
        case GLFW_KEY_2: light_bits ^= (1 << 1); break;
        case GLFW_KEY_3: light_bits ^= (1 << 2); break;
        case GLFW_KEY_4: light_bits ^= (1 << 3); break;
        case GLFW_KEY_5: light_bits ^= (1 << 4); break;
        case GLFW_KEY_6: light_bits ^= (1 << 5); break;
        case GLFW_KEY_7: light_bits ^= (1 << 6); break;
        case GLFW_KEY_8: light_bits ^= (1 << 7); break;
        default:
            break;
        }
    }
}

void MouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    float x_offset = x_pos - last_x;
    float y_offset = y_pos - last_y;
    last_x = x_pos;
    last_y = y_pos;

    float sensitivity = 0.1;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    camera.Rotate(glm::vec3(x_offset, y_offset, 0));
}

void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera.Zoom(y_offset);
}

bool IsLightOn(int index)
{
    return (light_bits >> index) & 1;
}

int main()
{
    // ---------------------gl初始化--------------------------
    // glfw初始化，采用的GL版本为3.3Core版
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建GL窗口
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HRIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 指定上下文为当前窗口
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetKeyCallback(window, KeyCallback);

    // 初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed!" << std::endl;
        return -1;
    }

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    camera.flip_y = true;

    Shader lamp_shader = Shader("res/shader/DrawLamp.vs", "res/shader/DrawLamp.fs");
    Shader cube_shader = Shader("res/shader/DrawCube.vs", "res/shader/DrawCube.fs");

    // 顶点数据
    float vertices[] = {
        // positions           // normals         // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
                                     
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
                                           
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                            
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                          
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    glm::vec3 point_light_positions[] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, -2.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, -3.0f),
        glm::vec3(-3.5f, 0.0f, 0.0f)
    };

    glm::vec3 light_colors[] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.5f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.5f, 0.0f, 0.5f)
    };

    DirectLight dirLight = DirectLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
    PointLight pointLights[6];
    // 点光源
    for (int i = 0; i < 6; i++)
    {
        pointLights[i] = PointLight(point_light_positions[i], 0.05f * light_colors[i], 0.8f * light_colors[i], light_colors[i], 1.0f, 0.09f, 0.032f);
    }
    SpotLight spotLight = SpotLight(camera.position, camera.forward, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f),
        1.0f, 0.09f, 0.032f, cos(glm::radians(12.5f)), cos(glm::radians(15.0f)));

    // ---------------------绑定顶点数组对象----------------------
    unsigned int vbo, lamp_vao, cube_vao;
    glGenVertexArrays(1, &cube_vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cube_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &lamp_vao);
    glBindVertexArray(lamp_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    cube_shader.Use();
    unsigned int diffuse_map = Texture::LoadTextureFromFile("res/texture/container2.png");
    unsigned int specular_map = Texture::LoadTextureFromFile("res/texture/container2_specular.png");
    cube_shader.SetInt("material.diffuse", 0);
    cube_shader.SetInt("material.specular", 1);

    last_frame = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        ProcessInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube_shader.Use();
        cube_shader.SetVec3("viewPos", camera.position);
        cube_shader.SetFloat("material.shininess", 32.0f);

        // 平行光光源
        dirLight.SetOn(IsLightOn(0));
        dirLight.Draw(cube_shader, "dirLight");        

        // 点光源
        for (int i = 0; i < 6; i++)
        {
            std::stringstream name_stream;
            name_stream << "pointLights[" << i << "]";
            std::string name = name_stream.str();
            pointLights[i].SetOn(IsLightOn(i + 1));
            pointLights[i].Draw(cube_shader, name);
        }

        // 手电筒光源
        spotLight.SetOn(IsLightOn(7));
        spotLight.SetPosition(camera.position);
        spotLight.SetDirection(camera.forward);
        spotLight.Draw(cube_shader, "spotLight");

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), SCREEN_WIDTH / SCREEN_HRIGHT, 0.1f, 100.0f);
        cube_shader.SetMat4("projection", projection);
        cube_shader.SetMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_map);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_map);
        glBindVertexArray(cube_vao);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, current_frame, glm::vec3(1.0f, 0.3f, 0.5f));
        cube_shader.SetMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lamp_shader.Use();
        lamp_shader.SetMat4("projection", projection);
        lamp_shader.SetMat4("view", view);
        glBindVertexArray(lamp_vao);
        for (int i = 0; i < 6; i++)
        {
            if (IsLightOn(i + 1))
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, point_light_positions[i]);
                model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
                lamp_shader.SetMat4("model", model);
                lamp_shader.SetVec3("lightColor", light_colors[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lamp_vao);
    glDeleteVertexArrays(1, &cube_vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}