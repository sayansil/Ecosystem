#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stdio.h>
#include <implot.h>
#include <vector>
#include <algorithm>
#include <random>

#include <GL/glew.h>            // Initialize with glewInit()
#include <GLFW/glfw3.h>

#include <god.hpp>
#include <iostream>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // ------------------ Window creation and handling starts  ------------------

    int size_X = 1400;
    int size_Y = 720;

    unsigned int initial_organism_count = 200;
    unsigned int years_to_simulate = 20;
    std::string kingdom = "animal";
    std::string species = "deer";

    God allah;
    allah.reset_species(kingdom + "/" + species);

    for (int i = 0; i < initial_organism_count; i++)
    {
        allah.spawn_organism(std::make_shared<Animal>(
                    species, "OG-" + std::to_string(i), 10));
    }

    int k = years_to_simulate;

    std::vector<int> population;
    std::vector<int> avg_age;
    std::vector<double> avg_height;
    std::vector<double> avg_weight;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(size_X, size_Y, "Ecosystem", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    bool err = glewInit() != GLEW_OK;

    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // ------------------ Window creation and handling starts  ------------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool started = false;
    ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y));
        ImGui::SetWindowSize(ImVec2(size_X, size_Y));
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowMenuButtonPosition = ImGuiDir_None;


        ImGui::Begin("Ecosystem Health");

        ImGui::Text("The Ecosystem will start with the following characteristics:");
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::Text("Species: %s", species.c_str());
        ImGui::Text("Initial Population: %d", initial_organism_count);
        ImGui::Text("Years to simulate: %d", years_to_simulate);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        if (started)
        {
            std::vector<int> x(years_to_simulate - k);
            std::iota(x.begin(), x.end(), 1);

            if(ImPlot::BeginPlot("Population"))
            {
                ImPlot::PushColormap(ImPlotColormap_Twilight);
                ImPlot::PlotLine("Line Plot", x.data(), population.data(), years_to_simulate);
                ImPlot::EndPlot();
            }
        }
        else
        {
            started = ImGui::Button("Start");
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Text("%.1f fps", ImGui::GetIO().Framerate);
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);


        if (k && started)
        {
            allah.happy_new_year(false);
            population.push_back(stat_fetcher::get_population(allah.organisms));
            avg_age.push_back(stat_fetcher::get_population(allah.organisms));
            avg_height.push_back(stat_fetcher::get_stat_average(allah.organisms, "height"));
            avg_weight.push_back(stat_fetcher::get_stat_average(allah.organisms, "weight"));
            k--;
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
