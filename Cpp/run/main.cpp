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

    int size_X = 1920;
    int size_Y = 1080;

    unsigned int initial_organism_count = 200;
    unsigned int years_to_simulate = 200;
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
    int max_population = 0;
    int max_age = 0;
    double max_height = 0;
    double max_weight = 0;
    int min_population = 0;
    int min_age = 0;
    double min_height = 0;
    double min_weight = 0;

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
        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        if (started)
        {
            ImGui::Text("Current Year of Simulation: %d", years_to_simulate - k);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            std::vector<int> x(years_to_simulate - k);
            std::iota(x.begin(), x.end(), 1);

            if (ImGui::BeginTable("Plots", 2))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                // Table -(0, 0)
                ImPlot::SetNextPlotLimits(1, years_to_simulate, min_population, max_population, ImGuiCond_Always);
                if(ImPlot::BeginPlot("Population"))
                {
                    ImPlot::PushColormap(ImPlotColormap_Twilight);
                    ImPlot::PlotLine("", x.data(), population.data(), x.size());
                    ImPlot::EndPlot();
                }

                ImGui::TableSetColumnIndex(1);

                // Table -(0, 1)
                ImPlot::SetNextPlotLimits(1, years_to_simulate, min_age, max_age, ImGuiCond_Always);
                if(ImPlot::BeginPlot("Average Age"))
                {
                    ImPlot::PushColormap(ImPlotColormap_Twilight);
                    ImPlot::PlotLine("", x.data(), avg_age.data(), x.size());
                    ImPlot::EndPlot();
                }

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                // Table -(1, 0)
                ImPlot::SetNextPlotLimits(1, years_to_simulate, min_height, max_height, ImGuiCond_Always);
                if(ImPlot::BeginPlot("Average Height"))
                {
                    ImPlot::PushColormap(ImPlotColormap_Twilight);
                    ImPlot::PlotLine("", (double*)x.data(), avg_height.data(), x.size());
                    ImPlot::EndPlot();
                }

                ImGui::TableSetColumnIndex(1);

                // Table -(1, 1)
                ImPlot::SetNextPlotLimits(1, years_to_simulate, min_weight, max_weight, ImGuiCond_Always);
                if(ImPlot::BeginPlot("Average Weight"))
                {
                    ImPlot::PushColormap(ImPlotColormap_Twilight);
                    ImPlot::PlotLine("", (double*)x.data(), avg_weight.data(), x.size());
                    ImPlot::EndPlot();
                }

                ImGui::EndTable();
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
            avg_age.push_back(stat_fetcher::get_stat_average(allah.organisms, "age"));
            avg_height.push_back(stat_fetcher::get_stat_average(allah.organisms, "height"));
            avg_weight.push_back(stat_fetcher::get_stat_average(allah.organisms, "weight"));
            max_population = population.back() > max_population ? population.back() : max_population;
            max_age = avg_age.back() > max_age ? avg_age.back() : max_age;
            max_height = avg_height.back() > max_height ? avg_height.back() : max_height;
            max_weight = avg_weight.back() > max_weight ? avg_weight.back() : max_weight;
            min_population = population.back() < min_population || k == years_to_simulate ? population.back() : min_population;
            min_age = avg_age.back() < min_age || k == years_to_simulate ? avg_age.back() : min_age;
            min_height = avg_height.back() > min_height || k == years_to_simulate ? avg_height.back() : min_height;
            min_weight = avg_weight.back() > min_weight || k == years_to_simulate ? avg_weight.back() : min_weight;
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
