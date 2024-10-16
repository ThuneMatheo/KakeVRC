#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// was going to add authcookie switching for fast account logins but na no thanks :P

std::vector<std::pair<std::string, std::string>> savedAccounts;
bool needsUpdate = true;  // Flag to indicate whether the file needs to be updated

void AccountManager()
{
    ImGui::SetNextWindowSize(ImVec2(300, 450), ImGuiCond_Once);
    if (!ImGui::Begin(nignog1("AccountManager"), nullptr)) {
        ImGui::End();
        return;
    }

    static int currentaccount_idx = 0;

    std::string filePath = "C:\\KAKEData\\Client\\SavedAccounts.txt";

    // Check if the file needs to be updated
    if (needsUpdate) {
        // Clear the current accounts vector
        savedAccounts.clear();

        // Check if the file exists before attempting to read it
        if (std::filesystem::exists(filePath)) {
            // Read saved accounts from file
            std::ifstream inputFile(filePath);
            if (inputFile.is_open()) {
                std::string line;
                while (std::getline(inputFile, line)) {
                    // Split each line into a pair of username and password
                    std::istringstream iss(line);
                    std::string username, password;
                    if (std::getline(iss, username, ':') && std::getline(iss, password)) {
                        savedAccounts.emplace_back(username, password);
                    }
                }
                inputFile.close();
            }
        }
        else {
            // If the file does not exist, create it
            std::ofstream outputFile(filePath);
            outputFile.close();
        }

        // Reset the update flag
        needsUpdate = false;
    }

    // Display saved accounts in a ListBox
    if (ImGui::BeginListBox(nignog1("##AccountList"), ImVec2(-FLT_MIN, (ImGui::GetWindowHeight() * 0.5f))))
    {
        for (size_t i = 0; i < savedAccounts.size(); i++) {
            const auto& account = savedAccounts[i];
            bool isSelected = (i == currentaccount_idx);

            if (ImGui::Selectable(account.first.c_str(), isSelected)) {
                currentaccount_idx = static_cast<int>(i);
            }

            if (isSelected) {

            }
        }

        ImGui::EndListBox();
    }

    // Button to update saved accounts
    if (ImGui::Button(nignog1("Update Account List"))) {
        needsUpdate = true;
    }

    if (ImGui::Button(nignog1("Login To Selected Account")))
    {
        /*
        Functions_t::Game::VRCCoreAPIUser::Login(IL2CPP::String::New(savedAccounts[currentaccount_idx].first.c_str()), IL2CPP::String::New(savedAccounts[currentaccount_idx].second.c_str()));
        Functions_t::Game::VRCCoreAPIUser::Login2();*/

        wantedusername = savedAccounts[currentaccount_idx].first;
        wantedpassword = savedAccounts[currentaccount_idx].second;
    }

    if (ImGui::Button(nignog1("Sign Out of Current Account")))
    {
        Functions_t::Game::VRCCoreAPIUser::Logout();
    }

    ImGui::End();
}
