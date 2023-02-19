// Dear ImGui: standalone example application for GLFW + Metal, using
// programmable pipeline (GLFW is a cross-platform general purpose library for
// handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation,
// etc.) If you are new to Dear ImGui, read documentation from the docs/ folder
// + read the top of imgui.cpp. Read online:
// https://github.com/ocornut/imgui/tree/master/docs

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_metal.h>
#include <stdio.h>
#include <implot.h>
#include <fmt/core.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

#include <thread>
#include <setup.hpp>
#include "simulation.hpp"

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char **) {

  setup::setup();

  Simulation sim;

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
  // Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable
  // Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                      // Platform Windows

  // Setup style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  // Create window with graphics context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  float resolution_x = mode->width;
  float resolution_y = mode->height;
  float scale = 0.5;

  fmt::print("Detected current resolution: {}x{}\n", resolution_x, resolution_y);

  GLFWwindow *window = glfwCreateWindow(resolution_x * scale, resolution_y * scale, "Ecosystem Simulation", NULL, NULL);
  if (window == NULL)
    return 1;

  id<MTLDevice> device = MTLCreateSystemDefaultDevice();
  id<MTLCommandQueue> commandQueue = [device newCommandQueue];

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOther(window, true);
  ImGui_ImplMetal_Init(device);

  NSWindow *nswin = glfwGetCocoaWindow(window);
  CAMetalLayer *layer = [CAMetalLayer layer];
  layer.device = device;
  layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
  nswin.contentView.layer = layer;
  nswin.contentView.wantsLayer = YES;

  MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor new];

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  float clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

  std::thread runner([&sim](){sim.run_ecosystem_simulation();});

  // Main loop
  while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      layer.drawableSize = CGSizeMake(width, height);
      id<CAMetalDrawable> drawable = [layer nextDrawable];

      id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
      renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(
          clear_color[0] * clear_color[3], clear_color[1] * clear_color[3],
          clear_color[2] * clear_color[3], clear_color[3]);
      renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
      renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
      renderPassDescriptor.colorAttachments[0].storeAction =
          MTLStoreActionStore;
      id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer
          renderCommandEncoderWithDescriptor:renderPassDescriptor];
      [renderEncoder pushDebugGroup:@"ImGui demo"];

      // Start the Dear ImGui frame
      ImGui_ImplMetal_NewFrame(renderPassDescriptor);
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      sim.RenderUI();

      // Rendering
      ImGui::Render();
      ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer,
                                     renderEncoder);

      // Update and Render additional Platform Windows
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
      }

      [renderEncoder popDebugGroup];
      [renderEncoder endEncoding];

      [commandBuffer presentDrawable:drawable];
      [commandBuffer commit];
  }

  // Cleanup
  ImGui_ImplMetal_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  runner.join();
  return 0;
}
