#include "../state/state.cpp"
#include "../log/log.cpp"
#include <string>

using namespace std;

string WATERMARK = "                                        /$$                             /$$    \n                                       | $$                            | $$    \n  /$$$$$$$  /$$$$$$  /$$$$$$   /$$$$$$$| $$   /$$  /$$$$$$$  /$$$$$$  /$$$$$$  \n /$$_____/ /$$__  $$|____  $$ /$$_____/| $$  /$$/ /$$_____/ |____  $$|_  $$_/  \n| $$      | $$  \\__/ /$$$$$$$| $$      | $$$$$$/ | $$        /$$$$$$$  | $$    \n| $$      | $$      /$$__  $$| $$      | $$_  $$ | $$       /$$__  $$  | $$ /$$\n|  $$$$$$$| $$     |  $$$$$$$|  $$$$$$$| $$ \\  $$|  $$$$$$$|  $$$$$$$  |  $$$$/\n \\_______/|__/      \\_______/ \\_______/|__/  \\__/ \\_______/ \\_______/   \\___/\n";
string KITTY = "       _                        \n       \\`*-.                    \n        )  _`-.                 \n       .  : `. .                \n       : _   '  \\               \n       ; *` _.   `*-._          \n       `-.-'          `-.       \n         ;       `       `.     \n         :.       .        \\    \n         . \\  .   :   .-'   .   \n         '  `+.;  ;  '      :   \n         :  '  |    ;       ;-. \n         ; '   : :`-:     _.`* ;\n      .*' /  .*' ; .*`- +'  `*' \n     `*-*   `*-*  `*-*'";


void printHeader() {
    string header = createHeader();

    printf("%s", header.c_str());
}

void printWatermark() {
    printf("%s\n", WATERMARK.c_str());
}

void printKitty() {
    printf("%s\n", KITTY.c_str());
}

void printAuthor() {
    printf("\t\tby: nekumelon\n\n");
}

void printCracked(State state) {
    if (state.quiet) {
        return;
    }

    string cracked = createCracked(state);

    printf("%s", cracked.c_str());
}

void printBenchmark(State state) {
    if (state.quiet) {
        return;
    }

    string benchmark = createBenchmark(state);

    printf("%s", benchmark.c_str());
}

void printSizeInfo(State state) {
    if (state.quiet) {
        return;
    }

    string sizeInfo = createSizeInfo(state);

    printf("%s", sizeInfo.c_str());
}
