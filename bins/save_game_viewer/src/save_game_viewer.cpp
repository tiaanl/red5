#include <base/streams/file_input_stream.h>
#include <base/streams/file_output_stream.h>
#include <xwing/pilot_data.h>

#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  std::vector<U8> buffer;
  fs::path path = fs::path{R"(C:\xwing\TILO.PLT)"};

  {
    auto fileSize = fs::file_size(path);

    base::FileInputStream inStream{path};
    buffer.resize(fileSize);
    inStream.read(buffer.data(), buffer.size());
  }

  PilotData* pilotData = reinterpret_cast<PilotData*>(buffer.data());

  spdlog::info("normalHealth: {}", pilotData->normalHealth);
  spdlog::info("captured: {}", pilotData->captured);
  spdlog::info("killed: {}", pilotData->killed);
  spdlog::info("rank: {}", pilotData->rank);
  spdlog::info("tourOfDutyScore: {}", pilotData->tourOfDutyScore);
  spdlog::info("rookieNumber: {}", pilotData->rookieNumber);
  spdlog::info("corellianCross: {}", pilotData->corellianCross);
  spdlog::info("mantooineMedallion: {}", pilotData->mantooineMedallion);
  spdlog::info("starOfAlderaan: {}", pilotData->starOfAlderaan);

  pilotData->rank = Rank::FlightOfficer;
  pilotData->corellianCross = 1;
  pilotData->mantooineMedallion = 1;
  pilotData->starOfAlderaan = 1;

  {
    base::FileOutputStream outStream{path};
    outStream.write(buffer.data(), buffer.size());
  }
  
  return 0;
}
