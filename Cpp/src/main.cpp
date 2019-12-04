#include <herbivore.hpp>
#include <iostream>
#include <helper.hpp>
#include <arrayfire.h>
#include <tuple>

std::vector<std::pair<std::string, af::array>> get_data_for_years(const int &n)
{
    Deer obj(Deer::generate_random_chromosome(), 1, "");
    std::map<std::string, std::vector<float>> plot_map;
    for(int i = 0; i < n; i++)
    {
        auto tmp = obj.get_stats();
        for(auto it = tmp.begin(); it != tmp.end(); it++)
        {
            if(it->second.index() == 1)
            {
                plot_map[it->first].push_back(std::get<double>(it->second));
            }
        }
        obj.increment_age();
    }



    std::vector<std::pair<std::string, af::array>> plots;

    for(auto it = plot_map.begin(); it != plot_map.end(); it++)
        plots.push_back(std::pair<std::string, af::array>(it->first, af::array(it->second.size(), it->second.data())));

    plots.shrink_to_fit();
    return plots;
}

int main()
{
    Deer::initialize();

    int n = 50, m = 100;

    af::array X = af::iota(n);
    X += 1;

    using plot_type = std::vector<std::pair<std::string, af::array>>;

    std::vector<plot_type> plot_list(m);
    for(int i = 0; i < m; i++)
    {
        plot_list[i] = get_data_for_years(n);
    }

    af::Window win;
    win.grid((int)ceil(plot_list[0].size() / 2.0), 2);

    while(true)
    {
        for(auto& plots : plot_list)
            for(int i = 0; i < plots.size(); i++)
            {
                win(i / 2, i % 2).setAxesTitles("Age", plots[i].first.c_str());
                win(i / 2, i % 2).plot(X, plots[i].second);
            }
        win.show();
    }
}
