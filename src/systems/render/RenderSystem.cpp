#include "../../pch.h"
#include "RenderSystem.h"

#include "glm/ext/matrix_transform.hpp"
#include "../../components/render/CameraComponent.h"
#include "../../components/basic/HierarchyComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../components/world/WorldMapComponent.h"
#include "../../client/Engine.h"

RenderSystem::RenderSystem(entt::registry &registry)
        : m_registry(registry),
          m_shader(Shader::createShader(TRUERPG_RES_DIR "/shaders/g_buffer.vs", TRUERPG_RES_DIR "/shaders/g_buffer.fs")),
          m_uiShader(Shader::createShader(TRUERPG_RES_DIR "/shaders/ui.vs", TRUERPG_RES_DIR "/shaders/ui.fs")),
          m_batch(m_shader, 30000)
{
    auto& window = Engine::getWindow();
    window.getOnResize() += createEventHandler(*this, &RenderSystem::resize);
    createGBuffer(window.getWidth(), window.getHeight());
}

RenderSystem::~RenderSystem()
{
    for (const auto &system : m_subsystems)
    {
        delete system;
    }
    for (const auto &system : m_lightSubsystems)
    {
        delete system;
    }
    for (const auto &system : m_uiSubsystems)
    {
        delete system;
    }
}

void RenderSystem::draw()
{
    // Find the first camera
    auto cameraView = m_registry.view<CameraComponent>();
    if (cameraView.empty()) return;
    auto cameraComponent = m_registry.get<CameraComponent>(cameraView[0]);
    TransformComponent cameraTransform = Hierarchy::computeTransform({cameraView[0], &m_registry});

    // Geometry pass: render scene's geometry/color data into g-buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer.id);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 viewMatrix = glm::translate(glm::mat4(1), glm::vec3(-cameraTransform.position, 0));
    m_batch.setViewMatrix(viewMatrix);
    m_batch.setProjectionMatrix(cameraComponent.getProjectionMatrix());
    m_batch.setShader(m_shader);
    m_batch.begin();

    for (auto &system : m_subsystems)
    {
        system->draw(m_batch);
    }

    m_batch.end();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the g-buffer's content
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_ONE, GL_ONE);
    for (auto &system : m_lightSubsystems)
    {
        Shader lightShader = system->getShader();
        lightShader.use();
        lightShader.setUniform("gPosition", 0);
        lightShader.setUniform("gAlbedoSpec", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_gBuffer.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_gBuffer.gAlbedoSpec);

        system->draw();
    }

    // UI pass
    m_batch.setViewMatrix(viewMatrix);
    m_batch.setProjectionMatrix(cameraComponent.getProjectionMatrix());
    m_batch.setShader(m_uiShader);
    m_batch.begin();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto &system : m_uiSubsystems)
    {
        system->draw(m_batch);
    }

    m_batch.end();
}

void RenderSystem::update(float deltaTime)
{
    for (auto &system : m_subsystems)
    {
        system->update(deltaTime);
    }
    for (auto &system : m_lightSubsystems)
    {
        system->update(deltaTime);
    }
    for (auto &system : m_uiSubsystems)
    {
        system->update(deltaTime);
    }
    draw();
}

void RenderSystem::destroy()
{
    for (auto &system : m_subsystems)
    {
        system->destroy();
    }
    for (auto &system : m_lightSubsystems)
    {
        system->destroy();
    }
    for (auto &system : m_uiSubsystems)
    {
        system->destroy();
    }
    m_batch.destroy();
    m_shader.destroy();
    m_uiShader.destroy();
}

// Every time when the window size is changed (by user or OS), this callback function is invoked
void RenderSystem::resize(int width, int height)
{
    glViewport(0, 0, width, height);
    createGBuffer(width, height);
}

void RenderSystem::createGBuffer(int width, int height)
{
    // TODO: create a separate class for g-buffer
    // Create g-buffer
    glGenFramebuffers(1, &m_gBuffer.id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer.id);

    // position color buffer
    glGenTextures(1, &m_gBuffer.gPosition);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer.gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gBuffer.gPosition, 0);

    // color + specular color buffer
    glGenTextures(1, &m_gBuffer.gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer.gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gBuffer.gAlbedoSpec, 0);

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
