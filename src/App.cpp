#include "App.hpp"

App::App()
{
  Engine::Log("Object Made");
}
App::~App()
{
  Engine::Log("Object Destroyed");
}

void App::Run()
{
  if (appState == AppState::ON)
    Engine::FatalError("App already running.");

  Engine::Init();

  unsigned int windowFlags = 0;

  // windowFlags |= Engine::WindowFlags::FULLSCREEN;

  // windowFlags |= Engine::WindowFlags::BORDERLESS;

  window.Create("Engine", 800, 600, windowFlags);

  Load();

  appState = AppState::ON;

  Loop();
}

void App::Load()
{
  // build and compile shader program
  shader.Compile("assets/shaders/3.1.shader.vs", "assets/shaders/3.1.shader.fs");
  shader.Link();

  float vertices[] = {
      // first triangle
      -0.5f, -0.5f, 0.0f, // left
      0.5f, -0.5f, 0.0f,  // right
      0.0f, 0.5f, 0.0f,   // top
      -0.25f, 0.f, 0.0f,  // left middle
      0.0f, -0.5f, 0.0f,  // center bottom
      0.25f, 0.0f, 0.0f,  // right middle
  };

  // VBO VAO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // binding vertex array object
  glBindVertexArray(VAO);
  // copying vertices array to a buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // setting vertex attribute pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0); // unbind vertex

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void App::Loop()
{
  while (appState == AppState::ON)
  {
    Update();
    Draw();
    // Get SDL to swap our buffer
    window.SwapBuffer();
    LateUpdate();
    FixedUpdate(0.0f);
    InputUpdate();
  }
}
void App::Update() {}
void App::Draw()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // take care to activate shader program before calling to uniforms
  shader.Use();

  // update shader uniform
  double timeValue = SDL_GetTicks() / 1000;
  float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
  // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
  // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

  shader.SetVec4("ourColor", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));

  // rendering triangle to window
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
  
  shader.UnUse();
}
void App::LateUpdate() {}
void App::FixedUpdate(float _delta_time) {}
void App::InputUpdate()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      appState = AppState::OFF;
      break;
    case SDL_MOUSEMOTION:
      break;
    case SDL_KEYUP:
      break;
    case SDL_KEYDOWN:
      break;
    }
  }
}