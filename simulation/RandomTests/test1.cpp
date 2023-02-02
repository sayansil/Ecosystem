#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <species_report_generated.h>

#include <helper.hpp>
#include <nlohmann/json.hpp>
#include <vector>

int main() {
    flatbuffers::FlatBufferBuilder builder;
    Visualisation::SpeciesReportBuilder report_builder(builder);

    std::vector<flatbuffers::Offset<Visualisation::MultiPlot>> stdvecMultiplot;

    std::vector<float> tmp = {0, 1, 2};

    {
        Visualisation::MultiPlotBuilder multiplot_builder(builder);
        std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
            stdvecSingleplot;

        // first single plot
        {
            Visualisation::SinglePlotBuilder singleplot_builder(builder);
            singleplot_builder.add_title(
                builder.CreateString("Theoretical max height"));
            singleplot_builder.add_x(builder.CreateVector(tmp));
            singleplot_builder.add_y(builder.CreateVector(tmp));
            stdvecSingleplot.push_back(singleplot_builder.Finish());
        }

        // second single plot
        {
            Visualisation::SinglePlotBuilder singleplot_builder(builder);
            singleplot_builder.add_title(
                builder.CreateString("Theoretical max width"));
            singleplot_builder.add_x(builder.CreateVector(tmp));
            singleplot_builder.add_y(builder.CreateVector(tmp));
            stdvecSingleplot.push_back(singleplot_builder.Finish());
        }

        // create first multiplot
        multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
        multiplot_builder.add_title(
            builder.CreateString("Theoretical max attributes"));
        stdvecMultiplot.push_back(multiplot_builder.Finish());
    }

    {
        Visualisation::MultiPlotBuilder multiplot_builder(builder);
        std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
            stdvecSingleplot;

        {
            Visualisation::SinglePlotBuilder singleplot_builder(builder);
            singleplot_builder.add_title(
                builder.CreateString("Theoretical max height"));
            singleplot_builder.add_x(builder.CreateVector(tmp));
            singleplot_builder.add_y(builder.CreateVector(tmp));
            stdvecSingleplot.push_back(singleplot_builder.Finish());
        }

        {
            Visualisation::SinglePlotBuilder singleplot_builder(builder);
            singleplot_builder.add_title(
                builder.CreateString("Theoretical max width"));
            singleplot_builder.add_x(builder.CreateVector(tmp));
            singleplot_builder.add_y(builder.CreateVector(tmp));
            stdvecSingleplot.push_back(singleplot_builder.Finish());
        }

        multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
        multiplot_builder.add_title(
            builder.CreateString("Theoretical max attributes 2"));
        stdvecMultiplot.push_back(multiplot_builder.Finish());
    }

    report_builder.add_title(builder.CreateString("My report"));
    report_builder.add_species(builder.CreateString("Human"));
    report_builder.add_plots(builder.CreateVectorOfSortedTables(
        stdvecMultiplot.data(), stdvecMultiplot.size()));
    builder.Finish(report_builder.Finish());

    auto buffer = builder.Release();
    builder.Clear();

    fmt::print("Buffer size: {}\n", buffer.size());
    flatbuffers::ToStringVisitor visitor("", true, "", true);
    flatbuffers::IterateFlatBuffer(
        buffer.data(), Visualisation::SpeciesReportTypeTable(), &visitor);
    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
}
