#pragma once

#include "Core.h"
#include "../cube_data.h"

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "imgui.h"

#include "glm/gtc/type_ptr.hpp"
#include "render/MeshRenderer.h"
#include "render/Renderer.h"
#include "render/TextureDescriptor.h"

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <random>

class TestBehavior : public hamu::Behavior
{
private:
    float fov_    = glm::radians(45.0f);
    float aspect_ = 1.0f * 800 / 600;
    float near_   = 0.1f;
    float far_    = 100.0f;

    float mouse_x_            = 800 / 2.0f;
    float mouse_y_            = 600 / 2.0f;
    float yaw_                = 0.0f; // 左右
    float pitch_              = 0.0f; // 上下
    float roll_               = 0.0f; // 滚转
    bool first_mouse_         = true;
    bool disable_mouse_input_ = false;

    hamu::Camera camera_;
    std::shared_ptr<hamu::Texture> texture_top_;
    std::shared_ptr<hamu::Texture> texture_side_;
    std::shared_ptr<hamu::Texture> texture_bottom_;
    std::shared_ptr<hamu::Material> material_top_;
    std::shared_ptr<hamu::Material> material_side_;
    std::shared_ptr<hamu::Material> material_bottom_;
    std::shared_ptr<hamu::Shader> shader_;

    hamu::VertexBuffer vbo_;
    hamu::IndexBuffer ebo_;
    std::shared_ptr<hamu::Mesh> mesh_;

    std::shared_ptr<hamu::MeshRenderer> mesh_renderer_;

    ImGuiIO& io_;
    hamu::Application& app_;

    std::vector<hamu::Transform> cube_transform;

    // C++ 允许私有重写虚函数, 用于接口约束, 保证派生类只能通过基类接口调用某些函数
    void Start() override {
        glEnable(GL_DEPTH_TEST); // 开启深度测试

        glEnable(GL_CULL_FACE);  // 开启面剔除
        glCullFace(GL_BACK);     // 剔除背面

        // 随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd()); // Mersenne Twister 引擎
        std::uniform_real_distribution<float> dist(-100.0f, 100.0f);


        cube_transform.reserve(1000);
        for (size_t i = 0; i < 1000; i++) {
            glm::vec3 pos(dist(gen), dist(gen), dist(gen));
            cube_transform.emplace_back(pos);
        }
    }

    void Update(float dt) override {

        ProcessInput(dt);

        // Clear
        // hamu::Renderer::ConfigureClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        hamu::Renderer::ConfigureClearFlags(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& trans : cube_transform) {
            // trans.setRotation(
            //     glm::rotate(trans.getRotation(), dt * glm::radians(90.0f), glm::normalize(trans.getPosition()))
            // );
            mesh_renderer_->OnRender(trans);
        }
    }

    void OnDrawImGui() override {
        static float f = glm::degrees(fov_);

        ImGui::Begin("Debug Window");

        ImGui::Text("Camera");

        ImGui::SliderFloat("FOV (float)", &f, 10.0f, 145.0f);
        camera_.SetFov(glm::radians(f));

        const auto& profile = hamu::Renderer::ProfileBlock();

        ImGui::Text("DrawCall: %zu", profile.drawcall_count);
        ImGui::Text("Mesh Binding: %zu", profile.mesh_binding_count);
        ImGui::Text("Material Binding: %zu", profile.material_binding_count);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);

        if (ImGui::Button("Close Application")) {
            app_.Close();
        };

        ImGui::End();
    }

    void OnCursorPosChanged(double xpos, double ypos) override {
        if (first_mouse_) {
            mouse_x_     = xpos;
            mouse_y_     = ypos;
            first_mouse_ = false;
        }

        if (disable_mouse_input_) {
            return;
        }

        float offset_x = xpos - mouse_x_;
        float offset_y = mouse_y_ - ypos; // ypos 的原点在左上角, 因此需要反转
        mouse_x_       = xpos;
        mouse_y_       = ypos;

        float sensitivity = 0.05f;
        offset_x *= sensitivity;
        offset_y *= sensitivity;

        yaw_ -= offset_x; // opengl 的坐标系是右手系, 因此鼠标向右移动时, 视角应该向左转
        pitch_ = std::clamp(pitch_ + offset_y, -89.0f, 89.0f);
        glm::vec3 eulerAngles(glm::radians(pitch_), glm::radians(yaw_), glm::radians(roll_));

        camera_.GetTransform().setRotation(glm::quat(eulerAngles));
    }

    void OnFramebufferResized(int width, int height) override {
        app_.SetWidth(width / app_.Scale());
        app_.SetHeight(height / app_.Scale());
        camera_.SetAspect(width, height);

        // Framebuffer 返回是物理像素大小, 不需要乘 scale
        glViewport(0, 0, width, height);
    }

    void ProcessInput(float deltaTime) {
        ImGuiIO& io = ImGui::GetIO();
        if (glfwGetKey(app_.Window(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            // glfwSetWindowShouldClose(app_.Window(), true);

            // 释放并显示鼠标
            disable_mouse_input_ = true;
            glfwSetInputMode(app_.Window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (glfwGetMouseButton(app_.Window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !io.WantCaptureMouse) {
            // 隐藏并捕捉鼠标
            glfwSetInputMode(app_.Window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            disable_mouse_input_ = false;
            first_mouse_         = true; // 防止跳动
        }

        float cameraSpeed = 20.0f * deltaTime; // adjust accordingly
        glm::vec3 forward = camera_.GetTransform().Forward();
        glm::vec3 up      = camera_.GetTransform().Up();
        glm::vec3 right   = camera_.GetTransform().Right();
        glm::vec3 moveDir = glm::vec3(0.0f);
        if (glfwGetKey(app_.Window(), GLFW_KEY_W) == GLFW_PRESS) {
            moveDir += forward;
        }
        if (glfwGetKey(app_.Window(), GLFW_KEY_S) == GLFW_PRESS) {
            moveDir -= forward;
        }
        if (glfwGetKey(app_.Window(), GLFW_KEY_A) == GLFW_PRESS) {
            moveDir -= right;
        }
        if (glfwGetKey(app_.Window(), GLFW_KEY_D) == GLFW_PRESS) {
            moveDir += right;
        }
        if (glfwGetKey(app_.Window(), GLFW_KEY_E) == GLFW_PRESS) {
            moveDir += up;
        }
        if (glfwGetKey(app_.Window(), GLFW_KEY_Q) == GLFW_PRESS) {
            moveDir -= up;
        }

        // 如果不是零向量才移动, 否则计算出现 NaN, 污染所有的矩阵计算
        if (glm::length(moveDir) > 1e-5f) {
            const auto& pos = camera_.GetTransform().getPosition();
            camera_.GetTransform().setPosition(pos + glm::normalize(moveDir) * cameraSpeed);
        }
    }

    void Destroy() override { std::cout << "TestBehavior Destroy()" << std::endl; }


public:
    TestBehavior(hamu::Application& app) : io_(ImGui::GetIO()), app_(app) {

        // ---------------------------------- Mesh -------------------------------------
        vbo_.SetData(cube);
        ebo_.SetData(cube_indices);

        hamu::VertexLayout layout;
        layout.PushFloat(3); // pos
        layout.PushFloat(2); // texcoord

        mesh_ = std::make_shared<hamu::Mesh>();
        mesh_->AddVertexLayout(vbo_, layout);
        mesh_->SetIndexBuffer(ebo_);
        mesh_->SetSubMeshes({{0, 6}, {6, 6}, {12, 24}});

        mesh_renderer_ = std::make_shared<hamu::MeshRenderer>(mesh_);
        // ---------------------------------- Mesh -------------------------------------


        // -------------------------------- Material -----------------------------------
        hamu::TextureDescriptor descriptor {
            .warp_s     = GL_REPEAT,
            .warp_t     = GL_REPEAT,
            .min_filter = GL_NEAREST,
            .mag_filter = GL_NEAREST,
        };

        texture_top_ = std::make_shared<hamu::Texture>();
        texture_top_->Load("resource/textures/piston_top.png", true, descriptor);

        texture_side_ = std::make_shared<hamu::Texture>();
        texture_side_->Load("resource/textures/piston_side.png", true, descriptor);

        texture_bottom_ = std::make_shared<hamu::Texture>();
        texture_bottom_->Load("resource/textures/piston_bottom.png", true, descriptor);

        shader_ = std::make_shared<hamu::Shader>();
        shader_->Create("resource/shaders/demo02.vert", "resource/shaders/demo02.frag");

        material_top_ = std::make_shared<hamu::Material>();
        material_top_->SetShader(shader_).AddTexture("texture1", texture_top_).Bind();

        material_side_ = std::make_shared<hamu::Material>();
        material_side_->SetShader(shader_).AddTexture("texture1", texture_side_).Bind();

        material_bottom_ = std::make_shared<hamu::Material>();
        material_bottom_->SetShader(shader_).AddTexture("texture1", texture_bottom_).Bind();

        // -------------------------------- Material -----------------------------------


        mesh_renderer_->AddMaterial(material_bottom_);
        mesh_renderer_->AddMaterial(material_top_);
        mesh_renderer_->AddMaterial(material_side_);


        // 摄像机
        aspect_ = 1.0f * app.Width() / app.Height();
        camera_.SetAspect(aspect_);
        camera_.SetFov(fov_);
        camera_.SetNear(near_);
        camera_.SetFar(far_);
        camera_.GetTransform().setPosition(glm::vec3(0.0f, 0.0f, 7.0f));
        camera_.AsMainCamera();

        mouse_x_ = app.Width() / 2.0f;
        mouse_y_ = app.Height() / 2.0f;
    }
};