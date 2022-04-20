#include "radiation_handler.h"

#include "core/config.h"

#include "resources.h"

namespace satdump
{
    void RadiationViewerHandler::init()
    {
        products = (RadiationProducts *)ViewerHandler::products;

        for (int c = 0; c < products->channel_counts.size(); c++)
            select_channel_image_str += "Channel " + std::to_string(c + 1) + '\0';

        update();
    }

    void RadiationViewerHandler::update()
    {
        if (selected_visualization_id == 0)
        {
            image::Image<uint16_t> map;
            map.load_jpeg(resources::getResourcePath("maps/nasa.jpg").c_str());
            make_radiation_map(*products, select_channel_image_id, map, 5, image::LUT_jet<uint16_t>(), map_min, map_max);
            image_view.update(map);
        }
        else if (selected_visualization_id == 1)
        {
            graph_values.resize(products->channel_counts.size());
            for (int c = 0; c < products->channel_counts.size(); c++)
            {
                for (int i = 0; i < products->channel_counts[c].size(); i++)
                    graph_values[c].push_back(products->channel_counts[c][i]);
            }
        }
    }

    void RadiationViewerHandler::drawMenu()
    {
        if (ImGui::CollapsingHeader("Vizualisation"))
        {
            if (ImGui::RadioButton("Map", &selected_visualization_id, 0))
                update();
            if (ImGui::RadioButton("Graph", &selected_visualization_id, 1))
                update();

            if (selected_visualization_id == 0)
            {
                if (ImGui::Combo("###mapchannelcomboid", &select_channel_image_id, select_channel_image_str.c_str()))
                    update();
                ImGui::InputInt("Min", &map_min);
                ImGui::InputInt("Max", &map_max);
            }
        }
    }

    void RadiationViewerHandler::drawContents(ImVec2 win_size)
    {
        if (selected_visualization_id == 0)
        {
            image_view.draw(win_size);
        }
        else if (selected_visualization_id == 1)
        {
            ImGui::BeginChild("RadiationPlotChild");
            for (int i = 0; i < products->channel_counts.size(); i++)
                ImGui::PlotLines(std::string("Channel " + std::to_string(i + 1)).c_str(), graph_values[i].data(), graph_values[i].size());
            ImGui::EndChild();
        }
    }

    float RadiationViewerHandler::drawTreeMenu()
    {
        return 0;
    }
}