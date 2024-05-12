#include "recomp_config.h"
#include "recomp_input.h"
#include "recomp_sound.h"
#include "../../ultramodern/config.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>

#if defined(_WIN32)
#include <Shlobj.h>
#elif defined(__linux__)
#include <unistd.h>
#include <pwd.h>
#endif

constexpr std::u8string_view general_filename = u8"general.json";
constexpr std::u8string_view graphics_filename = u8"graphics.json";
constexpr std::u8string_view controls_filename = u8"controls.json";
constexpr std::u8string_view sound_filename = u8"sound.json";

constexpr auto res_default            = ultramodern::Resolution::Auto;
constexpr auto hr_default             = ultramodern::HUDRatioMode::Clamp16x9;
constexpr auto api_default            = ultramodern::GraphicsApi::Auto;
constexpr auto ar_default             = RT64::UserConfiguration::AspectRatio::Expand;
constexpr auto msaa_default           = RT64::UserConfiguration::Antialiasing::MSAA2X;
constexpr auto rr_default             = RT64::UserConfiguration::RefreshRate::Display;
constexpr int ds_default              = 1;
constexpr int rr_manual_default       = 60;
constexpr bool developer_mode_default = false;

static bool is_steam_deck = false;

ultramodern::WindowMode wm_default() {
    return is_steam_deck ? ultramodern::WindowMode::Fullscreen : ultramodern::WindowMode::Windowed;
}

#ifdef __gnu_linux__
void detect_steam_deck() {
    // Check if the board vendor is Valve.
    std::ifstream board_vendor_file("/sys/devices/virtual/dmi/id/board_vendor");
    std::string line;
    if (std::getline(board_vendor_file, line).good() && line == "Valve") {
        is_steam_deck = true;
        return;
    }

    // Check if the SteamDeck variable is set to 1.
    const char* steam_deck_env = getenv("SteamDeck");
    if (steam_deck_env != nullptr && std::string{steam_deck_env} == "1") {
        is_steam_deck = true;
        return;
    }

    is_steam_deck = false;
    return;
}
#else
void detect_steam_deck() { is_steam_deck = false; }
#endif

template <typename T>
T from_or_default(const json& j, const std::string& key, T default_value) {
    T ret;
    auto find_it = j.find(key);
    if (find_it != j.end()) {
        find_it->get_to(ret);
    }
    else {
        ret = default_value;
    }
    
    return ret;
}

template <typename T>
void call_if_key_exists(void (*func)(T), const json& j, const std::string& key) {
    auto find_it = j.find(key);
    if (find_it != j.end()) {
        T val;
        find_it->get_to(val);
        func(val);
    }
}

namespace ultramodern {
    void to_json(json& j, const GraphicsConfig& config) {
        j = json{
            {"res_option",      config.res_option},
            {"wm_option",       config.wm_option},
            {"hr_option",       config.hr_option},
            {"api_option",      config.api_option},
            {"ds_option",       config.ds_option},
            {"ar_option",       config.ar_option},
            {"msaa_option",     config.msaa_option},
            {"rr_option",       config.rr_option},
            {"rr_manual_value", config.rr_manual_value},
            {"developer_mode",  config.developer_mode},
        };
    }

    void from_json(const json& j, GraphicsConfig& config) {
        config.res_option       = from_or_default(j, "res_option",      res_default);
        config.wm_option        = from_or_default(j, "wm_option",       wm_default());
        config.hr_option        = from_or_default(j, "hr_option",       hr_default);
        config.api_option       = from_or_default(j, "api_option",      api_default);
        config.ds_option        = from_or_default(j, "ds_option",       ds_default);
        config.ar_option        = from_or_default(j, "ar_option",       ar_default);
        config.msaa_option      = from_or_default(j, "msaa_option",     msaa_default);
        config.rr_option        = from_or_default(j, "rr_option",       rr_default);
        config.rr_manual_value  = from_or_default(j, "rr_manual_value", rr_manual_default);
        config.developer_mode   = from_or_default(j, "developer_mode",  developer_mode_default);
    }
}

namespace recomp {
    void to_json(json& j, const InputField& field) {
        j = json{ {"input_type", field.input_type}, {"input_id", field.input_id} };
    }

    void from_json(const json& j, InputField& field) {
        j.at("input_type").get_to(field.input_type);
        j.at("input_id").get_to(field.input_id);
    }
}

std::filesystem::path recomp::get_app_folder_path() {
   std::filesystem::path recomp_dir{};

#if defined(_WIN32)
   // Deduce local app data path.
   PWSTR known_path = NULL;
   HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &known_path);
   if (result == S_OK) {
       recomp_dir = std::filesystem::path{known_path} / recomp::program_id;
   }

   CoTaskMemFree(known_path);
#elif defined(__linux__)
   const char *homedir;

   if ((homedir = getenv("HOME")) == nullptr) {
       homedir = getpwuid(getuid())->pw_dir;
   }

   if (homedir != nullptr) {
       recomp_dir = std::filesystem::path{homedir} / (std::u8string{u8".config/"} + std::u8string{recomp::program_id});
   }
#endif

    return recomp_dir;
}

void save_general_config(const std::filesystem::path& path) {
    std::ofstream config_file{path};
    
    nlohmann::json config_json{};

    recomp::to_json(config_json["targeting_mode"], recomp::get_targeting_mode());
    recomp::to_json(config_json["background_input_mode"], recomp::get_background_input_mode());
    config_json["rumble_strength"] = recomp::get_rumble_strength();
    config_json["gyro_sensitivity"] = recomp::get_gyro_sensitivity();
    config_json["mouse_sensitivity"] = recomp::get_mouse_sensitivity();
    config_json["autosave_mode"] = recomp::get_autosave_mode();
    config_json["debug_mode"] = recomp::get_debug_mode_enabled();
    config_file << std::setw(4) << config_json;
}

void set_general_settings_from_json(const nlohmann::json& config_json) {
    recomp::set_targeting_mode(from_or_default(config_json, "targeting_mode", recomp::TargetingMode::Switch));
    recomp::set_background_input_mode(from_or_default(config_json, "background_input_mode", recomp::BackgroundInputMode::On));
    recomp::set_rumble_strength(from_or_default(config_json, "rumble_strength", 25));
    recomp::set_gyro_sensitivity(from_or_default(config_json, "gyro_sensitivity", 50));
    recomp::set_mouse_sensitivity(from_or_default(config_json, "mouse_sensitivity", is_steam_deck ? 50 : 0));
    recomp::set_autosave_mode(from_or_default(config_json, "autosave_mode", recomp::AutosaveMode::On));
    recomp::set_debug_mode_enabled(from_or_default(config_json, "debug_mode", false));
}

void load_general_config(const std::filesystem::path& path) {
    std::ifstream config_file{path};
    nlohmann::json config_json{};

    config_file >> config_json;

    set_general_settings_from_json(config_json);
}

void assign_mapping(recomp::InputDevice device, recomp::GameInput input, const std::vector<recomp::InputField>& value) {
    for (size_t binding_index = 0; binding_index < std::min(value.size(), recomp::bindings_per_input); binding_index++) {
        recomp::set_input_binding(input, binding_index, device, value[binding_index]);
    }
};

// same as assign_mapping, except will clear unassigned bindings if not in value
void assign_mapping_complete(recomp::InputDevice device, recomp::GameInput input, const std::vector<recomp::InputField>& value) {
    for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
        if (binding_index >= value.size()) {
            recomp::set_input_binding(input, binding_index, device, recomp::InputField{});
        } else {
            recomp::set_input_binding(input, binding_index, device, value[binding_index]);
        }
    }
};

void assign_all_mappings(recomp::InputDevice device, const recomp::DefaultN64Mappings& values) {
    assign_mapping_complete(device, recomp::GameInput::A, values.a);
    assign_mapping_complete(device, recomp::GameInput::B, values.b);
    assign_mapping_complete(device, recomp::GameInput::Z, values.z);
    assign_mapping_complete(device, recomp::GameInput::START, values.start);
    assign_mapping_complete(device, recomp::GameInput::DPAD_UP, values.dpad_up);
    assign_mapping_complete(device, recomp::GameInput::DPAD_DOWN, values.dpad_down);
    assign_mapping_complete(device, recomp::GameInput::DPAD_LEFT, values.dpad_left);
    assign_mapping_complete(device, recomp::GameInput::DPAD_RIGHT, values.dpad_right);
    assign_mapping_complete(device, recomp::GameInput::L, values.l);
    assign_mapping_complete(device, recomp::GameInput::R, values.r);
    assign_mapping_complete(device, recomp::GameInput::C_UP, values.c_up);
    assign_mapping_complete(device, recomp::GameInput::C_DOWN, values.c_down);
    assign_mapping_complete(device, recomp::GameInput::C_LEFT, values.c_left);
    assign_mapping_complete(device, recomp::GameInput::C_RIGHT, values.c_right);

    assign_mapping_complete(device, recomp::GameInput::X_AXIS_NEG, values.analog_left);
    assign_mapping_complete(device, recomp::GameInput::X_AXIS_POS, values.analog_right);
    assign_mapping_complete(device, recomp::GameInput::Y_AXIS_NEG, values.analog_down);
    assign_mapping_complete(device, recomp::GameInput::Y_AXIS_POS, values.analog_up);
};

void recomp::reset_input_bindings() {
    assign_all_mappings(recomp::InputDevice::Keyboard, recomp::default_n64_keyboard_mappings);
    assign_all_mappings(recomp::InputDevice::Controller, recomp::default_n64_controller_mappings);
}

void recomp::reset_cont_input_bindings() {
    assign_all_mappings(recomp::InputDevice::Controller, recomp::default_n64_controller_mappings);
}

void recomp::reset_kb_input_bindings() {
    assign_all_mappings(recomp::InputDevice::Keyboard, recomp::default_n64_keyboard_mappings);
}

void reset_graphics_options() {
    ultramodern::GraphicsConfig new_config{};
    new_config.res_option = res_default;
    new_config.wm_option = wm_default();
    new_config.hr_option = hr_default;
    new_config.ds_option = ds_default;
    new_config.ar_option = ar_default;
    new_config.msaa_option = msaa_default;
    new_config.rr_option = rr_default;
    new_config.rr_manual_value = rr_manual_default;
    new_config.developer_mode = developer_mode_default;
    ultramodern::set_graphics_config(new_config);
}

void save_graphics_config(const std::filesystem::path& path) {
    std::ofstream config_file{path};
    
    nlohmann::json config_json{};
    ultramodern::to_json(config_json, ultramodern::get_graphics_config());
    config_file << std::setw(4) << config_json;
}

void load_graphics_config(const std::filesystem::path& path) {
    std::ifstream config_file{path};
    nlohmann::json config_json{};

    config_file >> config_json;

    ultramodern::GraphicsConfig new_config{};
    ultramodern::from_json(config_json, new_config);
    ultramodern::set_graphics_config(new_config);
}

void add_input_bindings(nlohmann::json& out, recomp::GameInput input, recomp::InputDevice device) {
    const std::string& input_name = recomp::get_input_enum_name(input);
    nlohmann::json& out_array = out[input_name];
    out_array = nlohmann::json::array();
    for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
        out_array[binding_index] = recomp::get_input_binding(input, binding_index, device);
    }
};

void save_controls_config(const std::filesystem::path& path) {
    nlohmann::json config_json{};

    config_json["keyboard"] = {};
    config_json["controller"] = {};

    for (size_t i = 0; i < recomp::get_num_inputs(); i++) {
        recomp::GameInput cur_input = static_cast<recomp::GameInput>(i);

        add_input_bindings(config_json["keyboard"], cur_input, recomp::InputDevice::Keyboard);
        add_input_bindings(config_json["controller"], cur_input, recomp::InputDevice::Controller);
    }

    std::ofstream config_file{path};
    config_file << std::setw(4) << config_json;
}

bool load_input_device_from_json(const nlohmann::json& config_json, recomp::InputDevice device, const std::string& key) {
    // Check if the json object for the given key exists.
    auto find_it = config_json.find(key);
    if (find_it == config_json.end()) {
        return false;
    }

    const nlohmann::json& mappings_json = *find_it;

    for (size_t i = 0; i < recomp::get_num_inputs(); i++) {
        recomp::GameInput cur_input = static_cast<recomp::GameInput>(i);
        const std::string& input_name = recomp::get_input_enum_name(cur_input);

        // Check if the json object for the given input exists and that it's an array.
        auto find_input_it = mappings_json.find(input_name);
        if (find_input_it == mappings_json.end() || !find_input_it->is_array()) {
            continue;
        }
        const nlohmann::json& input_json = *find_input_it;

        // Deserialize all the bindings from the json array (up to the max number of bindings per input).
        for (size_t binding_index = 0; binding_index < std::min(recomp::bindings_per_input, input_json.size()); binding_index++) {
            recomp::InputField cur_field{};
            recomp::from_json(input_json[binding_index], cur_field);
            recomp::set_input_binding(cur_input, binding_index, device, cur_field);
        }
    }

    return true;
}

void load_controls_config(const std::filesystem::path& path) {
    std::ifstream config_file{path};
    nlohmann::json config_json{};

    config_file >> config_json;

    if (!load_input_device_from_json(config_json, recomp::InputDevice::Keyboard, "keyboard")) {
        assign_all_mappings(recomp::InputDevice::Keyboard, recomp::default_n64_keyboard_mappings);
    }

    if (!load_input_device_from_json(config_json, recomp::InputDevice::Controller, "controller")) {
        assign_all_mappings(recomp::InputDevice::Controller, recomp::default_n64_controller_mappings);
    }
}

void save_sound_config(const std::filesystem::path& path) {
    nlohmann::json config_json{};

    config_json["bgm_volume"] = recomp::get_bgm_volume();
    config_json["low_health_beeps"] = recomp::get_low_health_beeps_enabled();
    
    std::ofstream config_file{path};
    config_file << std::setw(4) << config_json;
}

void load_sound_config(const std::filesystem::path& path) {
    std::ifstream config_file{path};
    nlohmann::json config_json{};

    config_file >> config_json;

    
    recomp::reset_sound_settings();
    call_if_key_exists(recomp::set_bgm_volume, config_json, "bgm_volume");
    call_if_key_exists(recomp::set_low_health_beeps_enabled, config_json, "low_health_beeps");
}

void recomp::load_config() {
    detect_steam_deck();

    std::filesystem::path recomp_dir = recomp::get_app_folder_path();
    std::filesystem::path general_path = recomp_dir / general_filename;
    std::filesystem::path graphics_path = recomp_dir / graphics_filename;
    std::filesystem::path controls_path = recomp_dir / controls_filename;
    std::filesystem::path sound_path = recomp_dir / sound_filename;

    if (!recomp_dir.empty()) {
        std::filesystem::create_directories(recomp_dir);
    }

    if (std::filesystem::exists(general_path)) {
        load_general_config(general_path);
    }
    else {
        // Set the general settings from an empty json to use defaults.
        set_general_settings_from_json({});
        save_general_config(general_path);
    }

    if (std::filesystem::exists(graphics_path)) {
        load_graphics_config(graphics_path);
    }
    else {
        reset_graphics_options();
        save_graphics_config(graphics_path);
    }

    if (std::filesystem::exists(controls_path)) {
        load_controls_config(controls_path);
    }
    else {
        recomp::reset_input_bindings();
        save_controls_config(controls_path);
    }

    if (std::filesystem::exists(sound_path)) {
        load_sound_config(sound_path);
    }
    else {
        recomp::reset_sound_settings();
        save_sound_config(sound_path);
    }
}

void recomp::save_config() {
    std::filesystem::path recomp_dir = recomp::get_app_folder_path();

    if (recomp_dir.empty()) {
        return;
    }

    std::filesystem::create_directories(recomp_dir);

    save_general_config(recomp_dir / general_filename);
    save_graphics_config(recomp_dir / graphics_filename);
    save_controls_config(recomp_dir / controls_filename);
    save_sound_config(recomp_dir / sound_filename);
}
