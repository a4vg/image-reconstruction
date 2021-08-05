#include "Reconstructor.hpp"

Reconstructor::Reconstructor(std::string filePattern, std::string ext, int from, int to, glm::vec3 pointColor) {
	pointCloudModel.setColor(pointColor);
  for (int i=from; i<=to; ++i) {
    std::string infile = filePattern + std::to_string(i) + "." + ext; 
    std::string outfile = "output/out-" + std::to_string(i) + "." + ext; 
    pointCloudModel.loadImage(infile, outfile);
		pointCloudModel.layerN += 1.0/(to*1.3);

    std::cout << "Image " << i << " processed\n"; 
  }
}

void Reconstructor::visualize(glm::vec3 bgcolor) {
	visualizer.init(bgcolor, pointCloudModel);
	visualizer.visualize();
}