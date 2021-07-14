#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stdio.h>
#include <implot.h>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>

#include <GL/glew.h>            // Initialize with glewInit()
#include <GLFW/glfw3.h>

#include <god.hpp>
#include <iostream>
#include <thread>

struct PlotAttribute
{
    std::vector<double> data;
    std::string name;
    std::pair<double, double> limits;
    std::string query_name;
    double padding;

    PlotAttribute(const std::string& name, const std::string& query_name, const size_t& data_size)
    {
        this->name = name;
        this->query_name = query_name;
        this->data.reserve(data_size);
        this->limits = {std::numeric_limits<double>::max(), std::numeric_limits<double>::min()};
        this->padding = 0.0;
    }

    void fetchNextValue(ENTITY_MAP_TYPE& organisms)
    {
        if(query_name == "population")
            data.push_back(stat_fetcher::get_population(organisms));
        else
            data.push_back(stat_fetcher::get_stat_average(organisms, query_name));
    }
};

std::vector<PlotAttribute> all_plots;

const unsigned int initial_organism_count = 200;
const unsigned int years_to_simulate = 200;
const std::string kingdom = "animal";
const std::string species = "deer";

int k;
bool started, stopped;

God allah;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void run_ecosystem_simulation()
{
    while(!started);
    while(k && !stopped)
    {
        allah.happy_new_year(false);
        for(auto& i : all_plots)
        {
            i.fetchNextValue(allah.organisms);
            i.limits = {std::min(i.data.back(), i.limits.first), std::max(i.data.back(), i.limits.second)};
            double tmp = (i.limits.second - i.limits.first) * 0.1;
            i.padding = tmp == 0.0 ? 1.0 : tmp;
        }
        k--;
    }
}

int main(int, char**)
{
    // ------------------ Window creation and handling starts  ------------------

    int size_X = 1920;
    int size_Y = 1080;

    allah.reset_species(kingdom + "/" + species);

    for (int i = 0; i < initial_organism_count; i++)
    {
        allah.spawn_organism(std::make_shared<Animal>(
                    species, "OG-" + std::to_string(i), 10));
    }

    k = years_to_simulate;
    std::vector<std::pair<double, double>> limits(4, {std::numeric_limits<double>::max(), std::numeric_limits<double>::min()});

    all_plots.emplace_back("Population", "population", years_to_simulate);
    all_plots.emplace_back("Age", "age", years_to_simulate);
    all_plots.emplace_back("Height", "height", years_to_simulate);
    all_plots.emplace_back("Weight", "weight", years_to_simulate);
    all_plots.emplace_back("Vision Radius", "vision_radius", years_to_simulate);
    all_plots.emplace_back("Speed at Age", "max_speed_at_age", years_to_simulate);
    all_plots.emplace_back("Appetite at Age", "max_appetite_at_age", years_to_simulate);
    all_plots.emplace_back("Vitality at Age", "max_vitality_at_age", years_to_simulate);
    all_plots.emplace_back("Stamina at Age", "max_stamina_at_age", years_to_simulate);
    all_plots.emplace_back("Mutation Probability", "mutation_probability", years_to_simulate);
    all_plots.emplace_back("Conceiving Probability", "conceiving_probability", years_to_simulate);
    all_plots.emplace_back("Death Factor By Age", "age_death_factor", years_to_simulate);
    all_plots.emplace_back("Death Factor By Fitness", "fitness_death_factor", years_to_simulate);
    all_plots.emplace_back("Age v Fitness", "age_fitness_on_death_ratio", years_to_simulate);
    all_plots.emplace_back("Death Factor", "death_factor", years_to_simulate);
    all_plots.emplace_back("Max Age", "max_age", years_to_simulate);
    all_plots.emplace_back("Start of Mating Age", "mating_age_start", years_to_simulate);
    all_plots.emplace_back("End of Mating Age", "mating_age_end", years_to_simulate);
    all_plots.emplace_back("Offsprings Factor", "offsprings_factor", years_to_simulate);
    all_plots.emplace_back("Height On Stamina Dependancy", "height_on_stamina", years_to_simulate);
    all_plots.emplace_back("Weight On Stamina Dependancy", "weight_on_stamina", years_to_simulate);
    all_plots.emplace_back("Height On Vitality Dependancy", "height_on_vitality", years_to_simulate);
    all_plots.emplace_back("Weight On Vitality Dependancy", "weight_on_vitality", years_to_simulate);
    all_plots.emplace_back("Height On Speed Dependancy", "height_on_speed", years_to_simulate);
    all_plots.emplace_back("Weight On Speed Dependancy", "weight_on_speed", years_to_simulate);
    all_plots.emplace_back("Vitality On Speed Dependancy", "vitality_on_speed", years_to_simulate);
    all_plots.emplace_back("Vitality On Appetite Dependancy", "vitality_on_appetite", years_to_simulate);
    all_plots.emplace_back("Stamina On Appetite Dependancy", "stamina_on_appetite", years_to_simulate);
    all_plots.emplace_back("Generation", "generation", years_to_simulate);
    all_plots.emplace_back("Static Fitness", "static_fitness", years_to_simulate);
    all_plots.emplace_back("Immunity", "immunity", years_to_simulate);
    all_plots.emplace_back("Maximum Base Height", "theoretical_maximum_base_height", years_to_simulate);
    all_plots.emplace_back("Maximum Base Weight", "theoretical_maximum_base_weight", years_to_simulate);
    all_plots.emplace_back("Maximum Height", "theoretical_maximum_height", years_to_simulate);
    all_plots.emplace_back("Maximum Weight", "theoretical_maximum_weight", years_to_simulate);
    all_plots.emplace_back("Maximum Base Speed", "theoretical_maximum_base_speed", years_to_simulate);
    all_plots.emplace_back("Maximum Base Appetite", "theoretical_maximum_base_appetite", years_to_simulate);
    all_plots.emplace_back("Maximum Base Stamina", "theoretical_maximum_base_stamina", years_to_simulate);
    all_plots.emplace_back("Maximum Base Vitality", "theoretical_maximum_base_vitality", years_to_simulate);
    all_plots.emplace_back("Maximum Speed", "theoretical_maximum_speed", years_to_simulate);
    all_plots.emplace_back("Maximum Speed Multiplier", "theoretical_maximum_speed_multiplier", years_to_simulate);
    all_plots.emplace_back("Maximum Stamina Multiplier", "theoretical_maximum_stamina_multiplier", years_to_simulate);
    all_plots.emplace_back("Maximum Vitality Multiplier", "theoretical_maximum_vitality_multiplier", years_to_simulate);
    all_plots.emplace_back("Maximum Height Multiplier", "theoretical_maximum_height_multiplier", years_to_simulate);
    all_plots.emplace_back("Maximum Weight Multiplier", "theoretical_maximum_weight_multiplier", years_to_simulate);

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

    started = false;
    stopped = false;

    ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    std::thread runner(run_ecosystem_simulation);

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
            if (!stopped) {
                stopped = ImGui::Button("Stop");
            } else {
                ImGui::Text("Stopped the Simulation.");
            }
            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            ImGui::Text("Current Year of Simulation: %d", years_to_simulate - k);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            std::vector<double> x(years_to_simulate - k);
            std::iota(x.begin(), x.end(), 1);

            const int max_cols = 2;

            if (ImGui::BeginTable("Plots", max_cols))
            {
                for(int index = 0, cols = 0; index < all_plots.size(); index++, cols++)
                {
                    if(index % max_cols == 0)
                    {
                        ImGui::TableNextRow();
                        cols = 0;
                    }

                    ImGui::TableSetColumnIndex(cols);
                    ImPlot::SetNextPlotLimits(1, years_to_simulate, all_plots[index].limits.first - all_plots[index].padding, all_plots[index].limits.second + all_plots[index].padding, ImGuiCond_Always);
                    if(ImPlot::BeginPlot(all_plots[index].name.c_str()))
                    {
                        ImPlot::PushColormap(ImPlotColormap_Twilight);
                        ImPlot::PlotLine("", x.data(), all_plots[index].data.data(), x.size());
                        ImPlot::EndPlot();
                    }
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
    }

    runner.join();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
