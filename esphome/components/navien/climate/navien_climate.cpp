

#include "esphome.h"
#include "navien_climate.h"

namespace esphome {
namespace navien {
  
static const char *TAG = "navien.climate";

static const std::set<climate::ClimateMode> supported_modes = {climate::CLIMATE_MODE_OFF, climate::CLIMATE_MODE_HEAT};
  
void NavienClimate::setup(){
}
  
void NavienClimate::dump_config(){
}


climate::ClimateTraits NavienClimate::traits(){
    auto traits = climate::ClimateTraits();
    
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes(supported_modes);
    return traits;
}
  
void NavienClimate::control(const climate::ClimateCall &call){
  if (call.get_target_temperature().has_value()) {
    const float c = *call.get_target_temperature();
    float f = c * 9.0f / 5.0f + 32.0f;
    f = roundf(f);
    
    ESP_LOGD(TAG, "Setting target temperature to %f", target);
    parent->send_set_temp_cmd(f);
    this->target_temperature = (f - 32.0f) * 5.0f / 9.0f;
    this->publish_state();
  }
    
  //    std::string mode_id;
  if (call.get_mode().has_value()) {
    climate::ClimateMode mode = call.get_mode().value();
    ESP_LOGD(TAG, "Setting mode to %s", climate::climate_mode_to_string(mode));
    switch(mode){
    case climate::ClimateMode::CLIMATE_MODE_OFF:
      parent->send_turn_off_cmd();
      break;
    case climate::ClimateMode::CLIMATE_MODE_HEAT:
      parent->send_turn_on_cmd();
      break;
    default:
      ESP_LOGD(TAG, "Unsupported Mode (%s)", climate::climate_mode_to_string(mode));
    }
  }
}

  void NavienClimate::set_parent(Navien * parent_){
    this->parent = parent_;
    parent->set_climate(this);
  }
}
}
