#include <helper.hpp>
#include <fmt/core.h>
#include <vector>
#include <species_report_generated.h>

int main()
{
    flatbuffers::FlatBufferBuilder builder;
    Visualisation::SpeciesReportBuilder report_builder(builder);

    std::vector<flatbuffers::Offset<Visualisation::MultiPlot>> stdvecMultiplot;
    Visualisation::MultiPlotBuilder multiplot_builder(builder);

    std::vector<flatbuffers::Offset<Visualisation::SinglePlot>> stdvecSingleplot;
    Visualisation::SinglePlotBuilder singleplot_builder(builder);

    std::vector<float> tmp = {0, 1, 2};

    multiplot_builder.add_title(builder.CreateString("Theoretical max attributes"));
    singleplot_builder.add_title(builder.CreateString("Theoretical max height"));
    singleplot_builder.add_x(builder.CreateVector(tmp));
    singleplot_builder.add_y(builder.CreateVector(tmp));
    stdvecSingleplot.push_back(singleplot_builder.Finish());
    singleplot_builder.add_title(builder.CreateString("Theoretical max width"));
    singleplot_builder.add_x(builder.CreateVector(tmp));
    singleplot_builder.add_y(builder.CreateVector(tmp));
    stdvecSingleplot.push_back(singleplot_builder.Finish());
    multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
    stdvecMultiplot.push_back(multiplot_builder.Finish());

    multiplot_builder.add_title(builder.CreateString("Theoretical max attributes 2"));
    singleplot_builder.add_title(builder.CreateString("Theoretical max height"));
    singleplot_builder.add_x(builder.CreateVector(tmp));
    singleplot_builder.add_y(builder.CreateVector(tmp));
    stdvecSingleplot.push_back(singleplot_builder.Finish());
    singleplot_builder.add_title(builder.CreateString("Theoretical max width"));
    singleplot_builder.add_x(builder.CreateVector(tmp));
    singleplot_builder.add_y(builder.CreateVector(tmp));
    stdvecSingleplot.push_back(singleplot_builder.Finish());
    multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
    stdvecMultiplot.push_back(multiplot_builder.Finish());

    report_builder.add_title(builder.CreateString("My report"));
    report_builder.add_species(builder.CreateString("Human"));
    report_builder.add_plots(builder.CreateVectorOfSortedTables(stdvecMultiplot.data(), stdvecMultiplot.size()));
    builder.Finish(report_builder.Finish());

    auto buffer = builder.Release();
    builder.Clear();

    fmt::print("Buffer size: {}\n", buffer.size());
}
