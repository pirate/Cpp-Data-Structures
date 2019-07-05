#include <iostream>
#include <dirent.h>

using namespace std;

const string MUSIC_FOLDER = "/Users/squash/Desktop/Music";


// Helpers
string get_user_input(string prompt) {
    cout << "[?] " << prompt;
    string input_str;
    getline(cin, input_str);
    return input_str;
}

bool folder_exists(string path) {
    return opendir(path.c_str()) != NULL;
}

void list_directory_contents(string path) {
    struct dirent *pent = NULL;
    DIR* pdir = opendir(path.c_str());

    // iterate through all files in directory and print to cout
    while ((pent = (readdir(pdir)))) {
        if (pent == NULL)
            cout << "[X] Error occured while initializing pent info" << endl;

        string row = pent->d_name;

        if (row != "." && row != ".." && row != ".DS_Store")
            cout << pent->d_name << endl;
    }
    cout << endl;
}

void list_all_in_subdirectories(string path) {
    struct dirent *pent = NULL;
    DIR* pdir = opendir(path.c_str());

    // iterate through all files in directory
    while ((pent = (readdir(pdir)))) {
        if (pent == NULL)
            cout << "[X] Error occured while initializing pent info" << endl;

        string row = pent->d_name;

        // for each subfolder, pass to list_dir_cont to print its contents
        if (row != "." && row != ".." && row != ".DS_Store")
            list_directory_contents(path + "/" + row);
    }
}


// Output printing
void display_artists(string musicdir) {
    cout << "[i] Artists: " << endl;
    list_directory_contents(musicdir);
}

void display_albums(string musicdir, string artist_name) {
    cout << "[i] Albums by " << artist_name << ": " << endl;
    list_directory_contents(musicdir + "/" + artist_name);
}

void display_songs(string musicdir, string artist_name, string album_name) {
    if (album_name == "ALL") {
        cout << "[i] All songs by " << artist_name << ": " << endl;
        list_all_in_subdirectories(musicdir + "/" + artist_name);
    } else {
        cout << "[i] Songs by " << artist_name << " in " << album_name << ": " << endl;
        list_directory_contents(musicdir + "/" + artist_name + "/" + album_name);
    }
}


int main(int argc, const char* argv[]) {
    DIR* pdir = opendir(MUSIC_FOLDER.c_str());
    if (pdir == NULL) {
        cout << "[X] Cannot find music directory: " << MUSIC_FOLDER << endl;
        return 1;
    }

    struct dirent *pent = NULL;

    display_artists(MUSIC_FOLDER);
    string chosen_artist = get_user_input(
        "Enter the artist you would like to listen to: ");

    if (!folder_exists(MUSIC_FOLDER + "/" + chosen_artist)) {
        cout << "[X] That artist doesn't exist!" << endl;
        return 1;
    }

    display_albums(MUSIC_FOLDER, chosen_artist);
    string chosen_album = get_user_input(
        "Enter the album you would like to listen to, or ALL for all songs: ");

    if (chosen_album != "ALL" &&
        !folder_exists(MUSIC_FOLDER + "/" + chosen_artist + "/" + chosen_album)) {
        cout << "[X] That album doesn't exist!" << endl;
        return 1;
    }

    display_songs(MUSIC_FOLDER, chosen_artist, chosen_album);

    closedir(pdir);
    cout << "[√] Done!" << endl;
    return 0;
}
