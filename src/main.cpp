#include <stddef.h>

#include "PCH.h"
#include "Papyrus.h"
#include "RE/Skyrim.h"
#include "LSARFunc.h"

void InitializeLogging() {
    auto path = log_directory();
    if (!path) {
        report_and_fail("Unable to lookup SKSE logs directory.");
    }
    *path /= PluginDeclaration::GetSingleton()->GetName();
    *path += L".log";

    std::shared_ptr<spdlog::logger> log;
    if (IsDebuggerPresent()) {
        log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
    } else {
        log = std::make_shared<spdlog::logger>(
            "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
    }
    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
}

void LoadPluginMain() {
    const auto papyrus = SKSE::GetPapyrusInterface();
    if (!papyrus->Register(Papyrus::Register)) {
        logs::critical("Failed to register papyrus callback");
        return;
    }

	logs::info("Start LSARConfig::resetArmorAddon and LSARConfig::resetArmor.");
    LSARConfig::setModIndexes();
    LSARConfig::readIniFileARMA();
    LSARConfig::readIniFileARMO();
    LSARConfig::resetArmorAddon(2, 2, 2, 2, 2, 2, 2, 2);
    LSARConfig::resetArmor();

    //	LSARConfig::debugPrint();
    logs::info("Finish LSARConfig::resetArmorAddon and LSARConfig::resetArmor.");

    auto aa = LSARFunc::GetArmorAddonFromFormID(0x000179AE);
    auto str = LSARFunc::GetArmorAddonModelName(aa);
    logs::info("formID:{:08X}, modelname={}", 0x000179AE, str);
    logs::info("done.");
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    InitializeLogging();
    logs::info("Log Initialized. {} Load start.", SKSE::PluginDeclaration::GetSingleton()->GetName());
    SKSE::Init(skse);

    // This example prints "Hello, world!" to the Skyrim ~ console.
    // To view it, open the ~ console from the Skyrim Main Menu.
    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded) {
            logs::info("{} Loaded.", SKSE::PluginDeclaration::GetSingleton()->GetName());
            LoadPluginMain();
        }
    });

    return true;
}